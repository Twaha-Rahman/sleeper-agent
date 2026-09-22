function(AddClangTidy target)
  find_program(CLANG-TIDY_PATH clang-tidy REQUIRED)
  set_target_properties(${target}
    PROPERTIES CXX_CLANG_TIDY
    "${CLANG-TIDY_PATH};-checks=clang-analyzer-*,bugprone-*,readability-*,performance-*,cppcoreguidelines-*,modernize-*,cert-*,-cppcoreguidelines-owning-memory,-modernize-use-trailing-return-type,-readability-braces-around-statements,-readability-implicit-bool-conversion,-readability-magic-numbers,-readability-identifier-length;--warnings-as-errors=-*"
    # "${CLANG-TIDY_PATH};-checks=clang-analyzer-*,bugprone-*,readability-*,performance-*,cppcoreguidelines-*,modernize-*;--warnings-as-errors=*"
  )

  # If we modify the clang-tidy options above, remember to change them in the `.clang-tidy` file too 

endfunction()
