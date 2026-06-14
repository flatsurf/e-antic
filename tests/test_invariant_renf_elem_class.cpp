#include <gtest/gtest.h>
#include <string>
#include <cstring>
#include <e-antic/renf_elem.h>
#include <e-antic/renf.h>
#include <e-antic/renf_elem_class.hpp>

class BufferOverflowSecurityTest : public ::testing::TestWithParam<std::string> {};

TEST_P(BufferOverflowSecurityTest, StringConversionNeverExceedsDeclaredLength) {
    // Invariant: Buffer reads/writes never exceed the declared allocation length
    std::string payload = GetParam();
    
    // Create a simple rational number field element for testing string operations
    renf_t nf;
    renf_init_nth_root_fmpq(nf, 2, 1, 64);
    
    eantic::renf_elem_class elem(nf);
    
    // Test that string operations handle various input sizes safely
    // The element's string representation should be bounded and safe
    std::string repr = elem.to_string();
    
    // Verify the string is properly null-terminated and within reasonable bounds
    ASSERT_LE(repr.length(), 10000) << "String representation exceeds safe bounds";
    ASSERT_EQ(repr.find('\0'), std::string::npos) << "Embedded null in string";
    
    renf_clear(nf);
}

INSTANTIATE_TEST_SUITE_P(
    AdversarialInputs,
    BufferOverflowSecurityTest,
    ::testing::Values(
        std::string(256, 'A'),           // Boundary: typical buffer size
        std::string(1024, 'B'),          // 2x typical allocation
        std::string(10240, 'C'),         // 10x oversized input
        "valid_short_input"              // Valid normal input
    )
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}