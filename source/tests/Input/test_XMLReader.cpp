#include <gtest/gtest.h>
#include <Input/XMLReader.h>

using namespace PREW::Input;

TEST(TestXMLReader, DefaultConstructor) {
  XMLReader reader {"../scripts/test/test.xml","../scripts/processes"};
}
