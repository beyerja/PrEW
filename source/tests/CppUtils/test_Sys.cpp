#include <gtest/gtest.h>

#include <CppUtils/Sys.h>

using namespace PREW::CppUtils;

//------------------------------------------------------------------------------
// Tests for system helpers

TEST(TestSys, CheckPathExistance) {
  /** Test that existing path (file or directory) is found and non-existing not.
  **/
  ASSERT_EQ( 
    Sys::path_exists("../testdata/RK_examplefile_500_250_2018_04_03.root"), 
    true 
  );
  ASSERT_EQ( Sys::path_exists("../testdata"), true );
  ASSERT_EQ( Sys::path_exists("./obviously_wrong_path"), false );
}

//------------------------------------------------------------------------------