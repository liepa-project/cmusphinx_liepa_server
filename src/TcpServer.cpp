#include "TcpServer.h"
/**
 * 
 **/
TcpServer::TcpServer(int read_timeout) {
  server_desc_ = -1;
  client_desc_ = -1;
  samp_buf_ = NULL;
  buf_len_ = 0;
  read_timeout_ = 1000 * read_timeout;
}
/**
 * 
 **/
TcpServer::~TcpServer() {
  Disconnect();
  if (server_desc_ != -1)
    close(server_desc_);
  delete[] samp_buf_;
}
/**
 * 
 **/
void TcpServer::Disconnect() {
  if (client_desc_ != -1) {
    close(client_desc_);
    client_desc_ = -1;
  }
}


/**
 * 
 **/
int32 TcpServer::Accept() {
    printf("Waiting for client...\n");
    socklen_t len;

    len = sizeof(struct sockaddr);
    client_desc_ = accept(server_desc_, (struct sockaddr *) &h_addr_, &len);

    struct sockaddr_storage addr;
    char ipstr[20];

    len = sizeof addr;
    getpeername(client_desc_, (struct sockaddr *) &addr, &len);

    struct sockaddr_in *s = (struct sockaddr_in *) &addr;
    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);

    client_set_[0].fd = client_desc_;
    client_set_[0].events = POLLIN;

    printf("Waiting for client... %s\n", ipstr);

    return client_desc_;
}

/**
 * 
 **/
bool TcpServer::Listen(int32 port) {
  h_addr_.sin_addr.s_addr = INADDR_ANY;
  h_addr_.sin_port = htons(port);
  h_addr_.sin_family = AF_INET;

  server_desc_ = socket(AF_INET, SOCK_STREAM, 0);

  if (server_desc_ == -1) {
        printf("Cannot create TCP socket!\n");
        return false;
  }

  int32 flag = 1;
  int32 len = sizeof(int32);
  if (setsockopt(server_desc_, SOL_SOCKET, SO_REUSEADDR, &flag, len) == -1) {
        printf("Cannot set socket options!\n");
        return false;
  }

  if (bind(server_desc_, (struct sockaddr *) &h_addr_, sizeof(h_addr_)) == -1) {
        printf("Cannot bind to port: %d (is it taken?)\n", port);
        return false;
  }

  if (listen(server_desc_, 1) == -1) {
        printf("Cannot listen on port!\n");
        return false;
  }
    printf("TcpServer: Listening on port: %d \n", port);

  return true;

}

/**
 * 
 **/
bool TcpServer::ReadChunk(size_t len) {
  if (buf_len_ != len) {
    buf_len_ = len;
    delete[] samp_buf_;
    samp_buf_ = new int16[len];
  }

  ssize_t ret;
  int poll_ret;
  size_t to_read = len;
  has_read_ = 0;
  while (to_read > 0) {
    poll_ret = poll(client_set_, 1, read_timeout_);
    if (poll_ret == 0) {
        printf("Socket timeout! Disconnecting...\n");
      break;
    }
    if (poll_ret < 0) {
        printf("Socket error! Disconnecting...\n");
      break;
    }
    ret = read(client_desc_, static_cast<void *>(samp_buf_ + has_read_), to_read * sizeof(int16));
    // printf("[ReadChunk] Read bytes: %li\n", ret);
    printf(".");
    if (ret <= 0) {
        printf("Stream over...\n");
      break;
    }
    to_read -= ret / sizeof(int16);
    has_read_ += ret / sizeof(int16);
  }

  return has_read_ > 0;
}


vector<int16> TcpServer::GetChunk(){
    vector<int16> channelBuffer = vector<int16>(samp_buf_, samp_buf_+buf_len_);
    return channelBuffer;
}

/**
 * 
 **/
bool TcpServer::Write(const std::string &msg) {

  const char *p = msg.c_str();
  size_t to_write = msg.size();
  size_t wrote = 0;
  while (to_write > 0) {
    ssize_t ret = write(client_desc_, static_cast<const void *>(p + wrote), to_write);
    if (ret <= 0)
      return false;

    to_write -= ret;
    wrote += ret;
  }

  return true;
}

/**
 * 
 **/
bool TcpServer::WriteLn(const std::string &msg, const std::string &eol) {
  if (Write(msg))
    return Write(eol);
  else return false;
}