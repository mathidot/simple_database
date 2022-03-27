#include <gtest/gtest.h>
#include "src/defs.h"

TEST(BufferTest,HandleCreate){
    InputBuffer *buffer = new_input_buffer();
    EXPECT_EQ(sizeof(*buffer),sizeof(InputBuffer));
    EXPECT_EQ(buffer->buffer_length,0);
    EXPECT_EQ(buffer->input_length,0);
}
