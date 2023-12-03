IF(NOT EXISTS "/home/narraxas/Documents/Master1/M1/HAI719I - Programmation 3D/prog-3D-2023/template/build/thirdparties/assimp/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"/home/narraxas/Documents/Master1/M1/HAI719I - Programmation 3D/prog-3D-2023/template/build/thirdparties/assimp/install_manifest.txt\"")
ENDIF(NOT EXISTS "/home/narraxas/Documents/Master1/M1/HAI719I - Programmation 3D/prog-3D-2023/template/build/thirdparties/assimp/install_manifest.txt")

FILE(READ "/home/narraxas/Documents/Master1/M1/HAI719I - Programmation 3D/prog-3D-2023/template/build/thirdparties/assimp/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  EXEC_PROGRAM(
    "/snap/cmake/1356/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
    OUTPUT_VARIABLE rm_out
    RETURN_VALUE rm_retval
    )
  IF(NOT "${rm_retval}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
  ENDIF(NOT "${rm_retval}" STREQUAL 0)
ENDFOREACH(file)
