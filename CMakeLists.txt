cmake_minimum_required(VERSION 3.13)

project (cmusphinx_liepa_server C CXX)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

find_package (Threads REQUIRED)

include_directories(include)


set(SPINXBASE_HOME ${CMAKE_CURRENT_LIST_DIR}/cmusphinx/sphinxbase)
set(POCKETSPHINX_HOME ${CMAKE_CURRENT_LIST_DIR}/cmusphinx/pocketsphinx)

set(SPHINXBASE_SRC ${SPINXBASE_HOME}/src/libsphinxbase)
set(POCKETSPHINX_SRC ${POCKETSPHINX_HOME}/src/libpocketsphinx)

include_directories(${SPINXBASE_HOME}/include ${SPINXBASE_HOME}/include/sphinxbase ${POCKETSPHINX_HOME}/include)

set(SPHINXBASE_SOURCES
fe/fe_interface.c
fe/fe_noise.c
fe/fe_prespch_buf.c
fe/fe_sigproc.c
fe/fe_warp.c
fe/fe_warp_affine.c
fe/fe_warp_inverse_linear.c
fe/fe_warp_piecewise_linear.c
fe/fixlog.c
fe/yin.c
feat/agc.c
feat/cmn.c
feat/cmn_live.c
feat/feat.c
feat/lda.c
lm/fsg_model.c
lm/jsgf.c
lm/jsgf_parser.c
lm/jsgf_scanner.c
lm/lm_trie.c
lm/lm_trie_quant.c
lm/ngram_model.c
lm/ngram_model_set.c
lm/ngram_model_trie.c
lm/ngrams_raw.c
util/bio.c
util/bitarr.c
util/bitvec.c
util/blas_lite.c
util/case.c
util/ckd_alloc.c
util/cmd_ln.c
util/dtoa.c
util/err.c
util/errno.c
util/f2c_lite.c
util/filename.c
util/genrand.c
util/glist.c
util/hash_table.c
util/heap.c
util/listelem_alloc.c
util/logmath.c
util/matrix.c
util/mmio.c
util/pio.c
util/priority_queue.c
util/profile.c
util/sbthread.c
util/slamch.c
util/slapack_lite.c
util/strfuncs.c
)

set(SPHINXBASE_SOURCES_ABS)
foreach(source ${SPHINXBASE_SOURCES})
    list(APPEND SPHINXBASE_SOURCES_ABS "${SPHINXBASE_SRC}/${source}")
endforeach()


set(POCKETSPHINX_SOURCES
acmod.c
allphone_search.c
bin_mdef.c
blkarray_list.c
dict.c
dict2pid.c
fsg_history.c
fsg_lextree.c
fsg_search.c
hmm.c
kws_detections.c
kws_search.c
mdef.c
ms_gauden.c
ms_mgau.c
ms_senone.c
ngram_search.c
ngram_search_fwdflat.c
ngram_search_fwdtree.c
phone_loop_search.c
pocketsphinx.c
ps_alignment.c
ps_lattice.c
ps_mllr.c
ptm_mgau.c
s2_semi_mgau.c
state_align_search.c
tmat.c
vector.c
)

set(POCKETSPHINX_SOURCES_ABS)
foreach(source ${POCKETSPHINX_SOURCES})
    list(APPEND SPHINXBASE_SOURCES_ABS "${POCKETSPHINX_SRC}/${source}")
endforeach()




file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME}  ${SOURCES} ${SPHINXBASE_SOURCES_ABS} ${POCKETSPHINX_SOURCES_ABS})

target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
