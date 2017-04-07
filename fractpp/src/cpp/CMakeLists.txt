cmake_minimum_required ( VERSION 3.4.1 )

file ( GLOB_RECURSE src-fpp sources/*.cpp )

include_directories ( headers/ )

add_library ( lib-fpp
              SHARED
              ${src-fpp} )

find_library ( lib-log
               log )

find_library ( lib-gles2
               GLESv2 )

find_library ( lib-egl
               EGL )

find_library ( lib-android
               android )

target_link_libraries ( lib-fpp
                        ${lib-log}
                        ${lib-gles2}
                        ${lib-egl}
                        ${lib-android} )
