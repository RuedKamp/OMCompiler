cmake_minimum_required (VERSION 2.8.6)

project(${RTEulerName})
# add the solver default implementation library

add_library(${RTEulerName}_static STATIC RTEuler.cpp RTEulerSettings.cpp FactoryExport.cpp )
set_target_properties(${RTEulerName}_static PROPERTIES COMPILE_DEFINITIONS "RUNTIME_STATIC_LINKING")
include_directories(${SUNDIALS_INCLUDE_DIR}/cvodes ${SUNDIALS_INCLUDE_DIR}/nvector  ${SUNDIALS_INCLUDE_DIR}/sundials ${SUNDIALS_INCLUDE_DIR})

add_library(${RTEulerName} SHARED RTEuler.cpp RTEulerSettings.cpp FactoryExport.cpp )

if (UNIX)
	set_target_properties(${RTEulerName}_static PROPERTIES COMPILE_FLAGS -fPIC)
	set_target_properties(${RTEulerName} PROPERTIES COMPILE_FLAGS -fPIC)
endif(UNIX)

target_link_libraries (${RTEulerName} ${SolverName} ${Boost_LIBRARIES} ${LAPACK_LIBRARIES} )

install (TARGETS ${RTEulerName} ${RTEulerName}_static DESTINATION ${LIBINSTALLEXT})
