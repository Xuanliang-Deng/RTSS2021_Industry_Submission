/**
 * Authors:  Xuanliang Deng
 * Function: Unittests for common functions
 * Date:     Sept.2, 2021
 * Version:  v1
 * Usage:    $cmake .  -> $make  -> ./CommonTests
 * Comments: None
 *           
*/
#include "common.hpp"
#include "gtest/gtest.h"

// Test string to tag (int)
TEST(CommonTests, Test_str2tag){
	ASSERT_EQ(0, str_to_tag("CPU"));
    ASSERT_EQ(1, str_to_tag("GPU"));
    ASSERT_EQ(100, str_to_tag("TAG_UNDEFINED"));
    ASSERT_EQ(100, str_to_tag("ANY_OTHER_INPUTS"));
}

// Test tag (int) to string
TEST(CommonTests, Test_tag2str){
	ASSERT_EQ("CPU", tag_to_str(CPU));
    ASSERT_EQ("GPU", tag_to_str(GPU));
    ASSERT_EQ("TAG_UNDEFINED", tag_to_str(TAG_UNDEFINED));
    ASSERT_EQ("UNKNOWN", tag_to_str(PVA));
}

// Test vector_contains
TEST(CommonTests, Test_vecContains){
    std::vector<int> int_vect = {1,2,3,4,90};
    std::vector<double> dob_vect = {1.3,2.3,3.4,90.4};
    ASSERT_EQ(true, vector_contains(&int_vect, 1));
    ASSERT_EQ(true, vector_contains(&int_vect, 4));
    ASSERT_EQ(true, vector_contains(&int_vect, 90));
    ASSERT_EQ(false, vector_contains(&int_vect, 17));
    ASSERT_EQ(true, vector_contains(&dob_vect, 1.3));
    ASSERT_EQ(true, vector_contains(&dob_vect, 90.4));
    ASSERT_EQ(false, vector_contains(&dob_vect, 90.3));
    ASSERT_EQ(false, vector_contains(&dob_vect, 2.7));
}

// Test vector_remove
TEST(CommonTests, Test_vecRemove){
    std::vector<int> int_vect = {1,2,3,4,90};
    ASSERT_EQ(true, remove_from_vector(&int_vect, 90));
    ASSERT_EQ(true, remove_from_vector(&int_vect, 3));
    ASSERT_EQ(false, remove_from_vector(&int_vect, 3));
    ASSERT_EQ(false, remove_from_vector(&int_vect, 90));
    ASSERT_EQ(true, remove_from_vector(&int_vect, 4));
    ASSERT_EQ(2, int_vect.size());

    std::vector<double> dob_vect = {1.3,2.3,3.4,90.4};
    ASSERT_EQ(true, remove_from_vector(&dob_vect, 2.3));
    ASSERT_EQ(true, remove_from_vector(&dob_vect, 1.3));
    ASSERT_EQ(true, remove_from_vector(&dob_vect, 3.4));
    ASSERT_EQ(true, remove_from_vector(&dob_vect, 90.4));
    ASSERT_EQ(0, dob_vect.size());
}

// Test vector_copy
TEST(CommonTests, Test_vecCopy){
    std::vector<int> int_vect = {1,2,3,4,90};
    std::vector<int> int_vect_dst1;
    std::vector<int> int_vect_test = {1,2,5,4,90};
    std::vector<double> dob_vect = {1.3,2.3,3.4,90.4};
    std::vector<double> dob_vect_dst1;
    std::vector<double> dob_vect_test = {1.1,2.3,3.4,90.4};

    copy_vector(&int_vect, &int_vect_dst1);
    ASSERT_EQ(int_vect, int_vect_dst1);

    std::vector<int> int_vect_dst2;
    copy_vector(&int_vect, &int_vect_dst2);
    ASSERT_NE(int_vect_test, int_vect_dst2);

    copy_vector(&dob_vect, &dob_vect_dst1);
    ASSERT_EQ(dob_vect, dob_vect_dst1);

    std::vector<double> dob_vect_dst2;
    copy_vector(&dob_vect, &dob_vect_dst2);
    ASSERT_NE(dob_vect_test, dob_vect_dst2);
}

// Test vector minus
TEST(CommonTests, Test_vec_minus){
    std::vector<int> int_vect1 = {1,2,3,4,90};
    std::vector<int> int_vect2 = {1,2,4,80};
    std::vector<int> int_vect3;
    std::vector<int> int_vect_test1 = {3,90};
    vector_minus(&int_vect1, &int_vect2, &int_vect3);
    ASSERT_EQ(int_vect_test1, int_vect3);

    std::vector<double> dob_vect1 = {1.3,2.3,3.4,90.4};
    std::vector<double> dob_vect2 = {13.56,24.7};
    std::vector<double> dob_vect3;
    std::vector<double> dob_vect_test1 = {1.3,2.3,3.4,90.4};
    vector_minus(&dob_vect1, &dob_vect2, &dob_vect3);
    ASSERT_EQ(dob_vect_test1, dob_vect3);
}

// Test vector merge without duplicates
TEST(CommonTests, Test_merge_withoutDup){
    std::vector<int> int_vect1 = {1,2,3,4,90};
    std::vector<int> int_vect2 = {1,2,4,80};
    std::vector<int> int_vect_test1 = {1,2,3,4,90,80};
    merge_vectors_without_duplicates(&int_vect1, &int_vect2);
    ASSERT_EQ(int_vect_test1, int_vect1);

    std::vector<double> dob_vect1 = {1.3,2.3,3.4,90.4};
    std::vector<double> dob_vect2 = {13.56,24.7};
    std::vector<double> dob_vect_test1 = {13.56,24.7,1.3,2.3,3.4,90.4};
    merge_vectors_without_duplicates(&dob_vect2, &dob_vect1);
    ASSERT_EQ(dob_vect_test1, dob_vect2);
}

// Test vector merge with duplicates
TEST(CommonTests, Test_merge_withDup){
    std::vector<int> int_vect1 = {1,2,3,4,90};
    std::vector<int> int_vect2 = {1,2,4,80};
    std::vector<int> int_vect_test1 = {1,2,3,4,90,1,2,4,80};
    merge_vectors_with_duplicates(&int_vect1, &int_vect2);
    ASSERT_EQ(int_vect_test1, int_vect1);

    std::vector<double> dob_vect1 = {1.3,2.3,3.4,90.4};
    std::vector<double> dob_vect2 = {13.56,24.7};
    std::vector<double> dob_vect_test1 = {13.56,24.7,1.3,2.3,3.4,90.4};
    merge_vectors_without_duplicates(&dob_vect2, &dob_vect1);
    ASSERT_EQ(dob_vect_test1, dob_vect2);
}

int main(int argc, char **argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}