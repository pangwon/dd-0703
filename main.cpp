#include "gmock/gmock.h"
#include "device_driver.h"
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

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}