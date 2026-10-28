# gafro asks for Eigen's legacy include-directory variables even when Eigen is
# already present as a FetchContent target.  Supply those variables from the
# parent project instead of requiring a system-wide Eigen installation.
if(NOT TARGET Eigen3::Eigen)
    message(FATAL_ERROR "Eigen3::Eigen must be available before gafro")
endif()

set(EIGEN3_FOUND TRUE)
set(EIGEN3_INCLUDE_DIR "${GEOMETRIC_MECHANICS_EIGEN_SOURCE_DIR}")
set(EIGEN3_INCLUDE_DIRS "${GEOMETRIC_MECHANICS_EIGEN_SOURCE_DIR}")
set(EIGEN3_LIBRARIES Eigen3::Eigen)
set(EIGEN3_VERSION_STRING "3.4.0")
