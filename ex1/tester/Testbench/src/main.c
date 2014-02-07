#include <asf.h>

struct tc_module tc_instance;
struct tc_config config_tc;
i = 0;
n = 21;
int buttons[21] = {4, 1, 0, 6, 0, 1, 1, 0, 5, 3, 3, 3, 1, 4, 2, 0, 4, 6, 3, 5, 5};
int times[21] = {48906, 63101, 58139, 24083, 7660, 54105, 63679, 13047, 10800, 38869, 9090, 61318, 16798, 11706, 44234, 8963, 36186, 50684, 49422, 49080, 29346, 57735, 18900, 9081, 55089, 23893, 26658, 10380, 13600, 63478, 7974, 43725, 28116, 24201, 30466, 61461, 4777, 38875, 58482, 14938, 16391, 56975};


void tc_callback (struct tc_module *const module);

int main (void)
{
	system_init();

	/* Config timer */
	tc_get_config_defaults (&config_tc);
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV256;
	config_tc.wave_generation = TC_WAVE_GENERATION_MATCH_PWM;
	config_tc.size_specific.size_16_bit.compare_capture_channel[0] = times[i++];
	config_tc.channel_pwm_out_enabled[TC_COMPARE_CAPTURE_CHANNEL_0] = true;
	tc_init (&tc_instance, TC0, &config_tc);
	
	tc_register_callback (&tc_instance, tc_callback, TC_CALLBACK_CC_CHANNEL0);
	tc_enable_callback (&tc_instance, TC_CALLBACK_CC_CHANNEL0);
	
	tc_enable (&tc_instance);

	while (1);
}

void tc_callback (struct tc_module *const module)
{
	tc_set_top_value (&tc_instance, times[i]);
	port_pin_toggle_output_level (LED_0_PIN);
}