/**
 * Tests the values produced by the ADC along with a rolling average voltage
 */
#include <EVT/io/ADC.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;

namespace time = EVT::core::time;

uint32_t calcAvg(uint32_t lastTen[]) {
    uint32_t avg = 0;
    for (int i = 0; i < 10; i++) {
        avg += lastTen[i];
    }
    return avg / 10;
}

void switchMux(IO::GPIO& muxs3, IO::GPIO& muxs2, IO::GPIO& muxs1, uint8_t s3, uint8_t s2, uint8_t s1) {
    s3 == 0 ? muxs3.writePin(IO::GPIO::State::LOW) : muxs3.writePin(IO::GPIO::State::HIGH);
    s2 == 0 ? muxs2.writePin(IO::GPIO::State::LOW) : muxs2.writePin(IO::GPIO::State::HIGH);
    s1 == 0 ? muxs1.writePin(IO::GPIO::State::LOW) : muxs1.writePin(IO::GPIO::State::HIGH);
}

int main() {
    IO::init();

    IO::ADC& adc = IO::getADC<IO::Pin::PA_4>();
    IO::UART& uart = IO::getUART<IO::Pin::PB_10, IO::Pin::PB_11>(115200);

    IO::GPIO& muxs1 = IO::getGPIO<IO::Pin::PB_2>();
    IO::GPIO& muxs2 = IO::getGPIO<IO::Pin::PB_8>();
    IO::GPIO& muxs3 = IO::getGPIO<IO::Pin::PB_9>();

    uint32_t rollAvg = 0;
    uint32_t lastTen[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int counter = 0;
    unsigned int looper = 0;
    int32_t currV[] = {-1, -1, -1, -1};

    uart.printf("Starting ADC Voltage Testing -----\r\n");

    time::wait(500);

    while (1) {
        /**
         * The discrepency with the case and voltage index is due to an apparent miswiring in the TMS itself
         * The conversion to read temperatures in the correct order is as follows:
         * [Temp 1] is actually from [Temp 2]
         * [Temp 2] is actually from [Temp 3]
         * [Temp 3] is actually from [Temp 4]
         * [Temp 4] is actually from [Temp 1]
         */
        for (int i = 0; i < 100; i++) {
            switch (looper) {
            case 0:
                switchMux(muxs3, muxs2, muxs1, 0, 0, 0);
                time::wait(1);
                currV[1] = adc.readRaw();
                lastTen[counter] = currV[1];
                currV[0] = -1;
                currV[2] = -1;
                currV[3] = -1;
                break;
            case 1:
                switchMux(muxs3, muxs2, muxs1, 0, 0, 1);
                time::wait(1);
                currV[2] = adc.readRaw();
                lastTen[counter] = currV[2];
                currV[0] = -1;
                currV[1] = -1;
                currV[3] = -1;
                break;
            case 2:
                switchMux(muxs3, muxs2, muxs1, 0, 1, 0);
                time::wait(1);
                currV[3] = adc.readRaw();
                lastTen[counter] = currV[3];
                currV[0] = -1;
                currV[1] = -1;
                currV[2] = -1;
                break;
            case 3:
                switchMux(muxs3, muxs2, muxs1, 0, 1, 1);
                time::wait(1);
                currV[0] = adc.readRaw();
                lastTen[counter] = currV[0];
                currV[3] = -1;
                currV[1] = -1;
                currV[2] = -1;
                break;
            default:
                break;
            }
            if(i < 10) {
                rollAvg = 0;
            } else {
                rollAvg = calcAvg(lastTen);
            }

            uart.printf("%d, %d, %d, %d, ", currV[0], currV[1], currV[2], currV[3]);
            uart.printf("%d\r\n", rollAvg);

            if (counter == 10) {
                counter = 0;
            } else {
                counter++;
            }
        }
        looper = (looper + 1) % 4;
    }
}