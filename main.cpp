#include "gmock/gmock.h"
#include "device_driver.h"
#include "app.cpp"
#include <stdexcept>

using namespace testing;



class MockHardware : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DeviceDriverFixture : public Test {
public:
	MockHardware HARDWARE;
	DeviceDriver driver{ &HARDWARE };
	App app{ &driver };
};

TEST_F(DeviceDriverFixture, ReadFromHW) {
	EXPECT_CALL(HARDWARE, read)
		.Times(5)
		.WillRepeatedly(Return(0xAA));

	EXPECT_EQ(0xAA, driver.read(0xAA));
}

TEST_F(DeviceDriverFixture, ReadFailTest) {
	EXPECT_CALL(HARDWARE, read)
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAB));

	EXPECT_THROW(driver.read(0xAA), std::exception);
}

TEST_F(DeviceDriverFixture, WriteToHW) {
	EXPECT_CALL(HARDWARE, read(0xAA))
		.Times(1)
		.WillRepeatedly(Return(0xFF));

	driver.write(0xAA, 0xBB);
}

TEST_F(DeviceDriverFixture, WriteFailTest) {
	EXPECT_CALL(HARDWARE, read(0xAA))
		.WillOnce(Return(0xFA));

	EXPECT_THROW(driver.write(0xAA, 0xBB), std::exception);
}

TEST_F(DeviceDriverFixture, AppReadTest) {

	std::ostringstream oss;
	std::streambuf* orig_buf = std::cout.rdbuf(oss.rdbuf());

	EXPECT_CALL(HARDWARE, read(0xAA))
		.WillRepeatedly(Return(0xDE));
	EXPECT_CALL(HARDWARE, read(0xAB))
		.WillRepeatedly(Return(0xAD));
	EXPECT_CALL(HARDWARE, read(0xAC))
		.WillRepeatedly(Return(0xBE));
	EXPECT_CALL(HARDWARE, read(0xAD))
		.WillRepeatedly(Return(0xEF));

	app.readAndPrint(0xAA, 0xAD);
	std::cout.rdbuf(orig_buf);
	EXPECT_EQ(oss.str(), "0xDEADBEEF");
}

TEST_F(DeviceDriverFixture, AppWriteTest) {

}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}