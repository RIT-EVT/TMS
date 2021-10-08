
#include <EVT/io/GPIO.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/io/PWM.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

static IO::PWM& pwm = IO::getPWM<IO::Pin::PC_0>();
static IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

class PwmTest{
public:
    void StartPump(){
        this->setFrequency(150);
        pwm.setDutyCycle(.5);
        uart.printf("Pump is starting...\n\r");
    }

    static void setFrequency(float frequency){
        pwm.setPeriod(1/frequency);
    }
};



int main() {
    IO::init();

    PwmTest test;

    char buf[100];
    bool pumpRunning = false;

    while (1) {
        uart.printf("Enter Duty Cycle (0-100%%): ");
        uart.gets(buf, 100);
        uart.printf("\n\r");

        //uart.printf("\n\rBuf: %s\n\r",buf);

        if(buf[0]-48 >= 0 && buf[0]-48 <=9){
            if(pumpRunning==false){
            test.StartPump();
            pumpRunning=true;
            time::wait(3);
            }
            if(buf[1]-48 >= 0 && buf[1]-48 <=9) {
                float dutyIn = ((float) ((buf[0] - 48) * 10 + (buf[1] - 48))) / 100;
                //uart.printf("Input: %f\n\r", dutyIn);
                pwm.setDutyCycle(dutyIn);
                if (dutyIn == 0 || dutyIn == 1) {
                    pumpRunning = false;
                }
            }
            else {
                float dutyIn = ((float) buf[0] - 48) / 100;
                //uart.printf("Input: %f\n\r", dutyIn);
                pwm.setDutyCycle(dutyIn);
                if (dutyIn == 0 || dutyIn == 1) {
                    pumpRunning = false;
                }
            }
        }
        uart.printf("\n\r");
    }
}
