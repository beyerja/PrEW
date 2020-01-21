#include <gtest/gtest.h>

#include <CppUtils/Sys.h>

using namespace PREW::CppUtils;

//------------------------------------------------------------------------------
// Tests for system helpers

TEST(TestSys, FileFinding) {
  /** Test that existing files are found and non-existing not.
  **/
  ASSERT_EQ( Sys::file_exists("../testdata/RK_examplefile_500_250_2018_04_03.root"), true );
  ASSERT_EQ( Sys::file_exists("./obviously_wrong_file_path.root"), false );
}

//------------------------------------------------------------------------------
