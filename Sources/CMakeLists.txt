cmake_minimum_required( VERSION 2.8 )

#---------------------------------------------------------
# Set new app project
#---------------------------------------------------------
project( ExerciceFinal )

include(GlobalFunctions.cmake)
include(ExternalLibs.cmake)

#-----------------------------------------------------------
# Create header/source/hpp/ui/qrc files list
#-----------------------------------------------------------
file( GLOB_RECURSE header_files *.h )
file( GLOB_RECURSE source_files *.cpp )
file( GLOB_RECURSE hpp_files *.hpp )
file( GLOB_RECURSE ui_files *.ui )
file( GLOB_RECURSE qrc_files *.qrc )


#-----------------------------------------------------------
# Manage moc files
#-----------------------------------------------------------
find_moc_files( mocable_files ${header_files} )
qt5_wrap_cpp( moc_list ${mocable_files} )
qt5_wrap_ui( generated_ui_files ${ui_files} )
qt5_add_resources( generated_qrc_files ${qrc_files} )


#-----------------------------------------------------------
# source_group
#-----------------------------------------------------------
source_group_sources( ${header_files} )
source_group_sources( ${source_files} )
source_group_sources( ${hpp_files} )
source_group_mocs( ${moc_list} )
source_group_qrcs( ${qrc_files} )
source_group_generated_qrcs( ${generated_qrc_files} )


#-----------------------------------------------------------
# Include directories
#-----------------------------------------------------------
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )


#-----------------------------------------------------------
# Set hpp to header only
#-----------------------------------------------------------
manage_hpp_files( ${hpp_files} )


#-----------------------------------------------------------
# final link
#-----------------------------------------------------------
add_executable( ${PROJECT_NAME} ${header_files} ${source_files} ${hpp_files} ${moc_list} ${generated_ui_files} ${generated_qrc_files} )
target_link_libraries( ${PROJECT_NAME} ${EXTERNAL_LIBS_LIBRARIES} )
qt5_use_modules( ${PROJECT_NAME} Core Concurrent Widgets Xml Test)
