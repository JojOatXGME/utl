#include <string>

#include <gtest/gtest.h>

#include "utl/arguments.h"

using std::string;
using utl::Arguments;


TEST(ArgumentsParserTest, integer0)
{
	const char *argv[] = {"./myapp", "0"};
	Arguments args(2, argv);

	int arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(     0, arg);
}

TEST(ArgumentsParserTest, integerMinus1)
{
	const char *argv[] = {"./myapp", "-1"};
	Arguments args(2, argv);

	int arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(    -1, arg);
}

TEST(ArgumentsParserTest, integer50)
{
	const char *argv[] = {"./myapp", "50"};
	Arguments args(2, argv);

	int arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(    50, arg);
}

TEST(ArgumentsParserTest, integerFailEmpty)
{
	const char *argv[] = {"./myapp", ""};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

TEST(ArgumentsParserTest, integerFailSuffix)
{
	const char *argv[] = {"./myapp", "42Suffix"};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

TEST(ArgumentsParserTest, integerFailPrefix)
{
	const char *argv[] = {"./myapp", "Pre24"};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}
