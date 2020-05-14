/* global data needs to be initialized somewhere */
extern "C" void __call_constructors() {
	extern void (*__init_array_start)();
	extern void (*__init_array_end)();
	for (void (**p)() = &__init_array_start;
	     reinterpret_cast<unsigned long>(p) < reinterpret_cast<unsigned long>(&__init_array_end);
	     ++p) {
		(*p)();
	}
}

extern "C" void __call_destructors() {
	extern void (*__fini_array_start)();
	extern void (*__fini_array_end)();
	for (void (**p)() = &__fini_array_start;
	     reinterpret_cast<unsigned long>(p) < reinterpret_cast<unsigned long>(&__fini_array_end);
	     ++p) {
		(*p)();
	}
}
