#include <gtest/gtest.h>

#include <CppUtils/Sys.h>

using namespace PrEW::CppUtils;

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

TEST(TestSys, CheckFileWritability) {
  // Test checking if a file can written to
  EXPECT_EQ(Sys::file_writable(""), false);
  
  // Should be able to write to current dir and other existing dir
  EXPECT_EQ(Sys::file_writable("nonexistent_test_file.txt"), true);
  EXPECT_EQ(Sys::file_writable("./nonexistent_test_file.txt"), true);
  EXPECT_EQ(Sys::file_writable("../build/nonexistent_test_file.txt"), true);

  // Should be able to write to random (existent) Makefile in build dir
  EXPECT_EQ(Sys::file_writable("../build/Makefile"), true);
  
  // Should be able to write to file in root-level bin dir
  EXPECT_EQ(Sys::file_writable("/usr/testfile.txt"), false);
  
  // Should not be able to write to non-existent directory 
  EXPECT_EQ(Sys::file_writable("/gobblegobblgobble/test.txt"), false);
}

//------------------------------------------------------------------------------