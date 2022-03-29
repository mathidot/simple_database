#include <gtest/gtest.h>
extern "C"{
#include "src/defs.h"
}

TEST(BufferTest,HandleCreate){
    InputBuffer *buffer = new_input_buffer();
    EXPECT_EQ(sizeof(*buffer),sizeof(InputBuffer));
    EXPECT_EQ(buffer->buffer_length,0);
    EXPECT_EQ(buffer->input_length,0);
}

int main(int argc, char **argv) {
  printf("Running main() from %s\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();   
}