#include <string>

#include <gtest/gtest.h>

#include "utl/arguments.h"

using std::string;
using utl::Arguments;


utl::argr::withUnit<long> testUnit{
		{"k",  1000},
		{"M",  1000 * 1000},
		{"G",  1000 * 1000 * 1000},
		{"ki", 1024},
		{"Mi", 1024 * 1024},
		{"Gi", 1024 * 1024 * 1024}
};


// ---------------------------------------------------------------------------
// integer

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

// ---------------------------------------------------------------------------
// double

TEST(ArgumentsParserTest, double00)
{
	const char *argv[] = {"./myapp", "0.0"};
	Arguments args(2, argv);

	double arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(   0.0, arg);
}

TEST(ArgumentsParserTest, double01)
{
	const char *argv[] = {"./myapp", "0.1"};
	Arguments args(2, argv);

	double arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(   0.1, arg);
}

TEST(ArgumentsParserTest, doubleMinus11)
{
	const char *argv[] = {"./myapp", "-1.1"};
	Arguments args(2, argv);

	double arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(  -1.1, arg);
}

TEST(ArgumentsParserTest, double500)
{
	const char *argv[] = {"./myapp", "50."};
	Arguments args(2, argv);

	double arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(  50.0, arg);
}

TEST(ArgumentsParserTest, doubleFailEmpty)
{
	const char *argv[] = {"./myapp", ""};
	Arguments args(2, argv);

	double arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

TEST(ArgumentsParserTest, doubleFailSuffix)
{
	const char *argv[] = {"./myapp", "42+"};
	Arguments args(2, argv);

	double arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

TEST(ArgumentsParserTest, doubleFailPrefix)
{
	const char *argv[] = {"./myapp", "#24"};
	Arguments args(2, argv);

	double arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

TEST(ArgumentsParserTest, doubleFailDot)
{
	const char *argv[] = {"./myapp", "."};
	Arguments args(2, argv);

	double arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

// ---------------------------------------------------------------------------
// unit

TEST(ArgumentsParserTest, unitInteger0)
{
	const char *argv[] = {"./myapp", "0"};
	Arguments args(2, argv);

	int arg;
	EXPECT_TRUE(      args.getNextArgument(arg, testUnit));
	EXPECT_EQ(     0, arg);
}

TEST(ArgumentsParserTest, unitFloatMinus1)
{
	const char *argv[] = {"./myapp", "-1Mi"};
	Arguments args(2, argv);

	float arg;
	EXPECT_TRUE(      args.getNextArgument(arg, testUnit));
	EXPECT_EQ(-1048576, arg);
}

TEST(ArgumentsParserTest, unitDouble50)
{
	const char *argv[] = {"./myapp", "50k"};
	Arguments args(2, argv);

	double arg;
	EXPECT_TRUE(      args.getNextArgument(arg, testUnit));
	EXPECT_EQ( 50000, arg);
}

TEST(ArgumentsParserTest, unitFailEmpty)
{
	const char *argv[] = {"./myapp", ""};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg, testUnit), std::exception);
}

TEST(ArgumentsParserTest, unitFailEmptyUnit)
{
	const char *argv[] = {"./myapp", "1"};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg, testUnit), std::exception);
}

TEST(ArgumentsParserTest, unitFailPrefix)
{
	const char *argv[] = {"./myapp", "P42"};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg, testUnit), std::exception);
}

TEST(ArgumentsParserTest, integerFailNotFound)
{
	const char *argv[] = {"./myapp", "24L"};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg, testUnit), std::exception);
}
