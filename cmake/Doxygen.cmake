function(Doxygen target_name input)
  find_package(Doxygen)
  if (NOT DOXYGEN_FOUND)
    add_custom_target(${target_name} COMMAND false 
      COMMENT "Doxygen not found")
    return()
  endif()
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_HTML_OUTPUT
    ${CMAKE_BINARY_DIR}/docs/${target_name})

  file(MAKE_DIRECTORY "${DOXYGEN_HTML_OUTPUT}")

  doxygen_add_docs(${target_name}
      ${CMAKE_SOURCE_DIR}/src/${input}
      COMMENT "Generate HTML documentation"
  )
endfunction()
