set(doc-main "${CMAKE_CURRENT_LIST_DIR}/main.tex")

find_package(LATEX REQUIRED)
message(NOTICE "LATEX_COMPILER: ${LATEX_COMPILER}")
message(NOTICE "PDFLATEX_COMPILER: ${PDFLATEX_COMPILER}")
message(NOTICE "LATEX2HTML_CONVERTER: ${LATEX2HTML_CONVERTER}")

# ADD_CUSTOM_TARGET( latex-prebuild
#   COMMAND ${PDFLATEX_COMPILER}  -output-directory ${CURRENT_CMAKE_BINARY_DIR} -draftmode -interaction=nonstopmode ${doc-main}
#   COMMENT "Run Prebuild."
#   WORKING_DIRECTORY ${CURRENT_CMAKE_SOURCE_DIR}
#   DEPENDS ${doc-main})

ADD_CUSTOM_TARGET( latex-pdf ALL
  COMMAND ${PDFLATEX_COMPILER} -output-directory ${CMAKE_CURRENT_BINARY_DIR} -interaction=nonstopmode ${doc-main}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Assembling the final pdf file."
  DEPENDS  ${doc-main})

