#include <Arduino.h>

extern "C"
{
#include <ranbo/ranbo.h>
}

void setup()
{
    Serial.begin(9600);

    Serial.print("ranbo version: ");
    Serial.print(STEINWURF_RANBO_VERSION);
    Serial.print("\n");

    struct ranbo_xoshiro256ss generator;

    ranbo_xoshiro256ss_set_seed(&generator, 0U);

    if (ranbo_xoshiro256ss_generate(&generator) != 18060408633489136533U)
    {
        Serial.print("Unexpected random number!\n");
        return;
    }
    Serial.print("Expected random number!\n");
}

void loop()
{
}
