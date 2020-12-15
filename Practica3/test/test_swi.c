int main (void) {
	enable_isr();
	clock_gettime();
	disable_isr();
	enable_isr_fiq();
	disable_isr_fiq();
}