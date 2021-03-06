#include <gtest/gtest.h>
#include <Input/InputInfo.h>
#include <Input/InfoRKFile.h>

using namespace PrEW::Input;

//------------------------------------------------------------------------------
// Tests for InputInfo class and its possible implementations

TEST(TestInputInfo, CastCheck) {
  /** Check if an InfoRKFile object (subclass of InputInfo) can be successfully
      cast to an InputInfo object and back.
  **/
  InfoRKFile i {"a", "b", 250};
  InfoRKFile *i_ptr = &i;
  InputInfo *a_ptr = i_ptr;
  InfoRKFile *c_ptr = static_cast<InfoRKFile*>(a_ptr);
  
  ASSERT_EQ(c_ptr->m_file_path, "a"); 
  ASSERT_EQ(c_ptr->m_input_style, "b"); 
  ASSERT_EQ(c_ptr->m_energy, 250); 
}

//------------------------------------------------------------------------------
