#pragma once
#include <Arduino.h>
#include <stdarg.h>
#ifndef __AVR__
// No C++11 initializer_list in AVR
#include <initializer_list>
#endif

/*
 * Library to bundle multiple pins into a bus.
 * With overloaded implementations of:
 * pinMode
 * digitalWrite
 * digitalread
 */
template <uint8_t MaxWidth = 16>
class ParallelBus {
	public:
		// ===== Universal Width Check =====
		static_assert(MaxWidth <= 16, "Max bus width is 16 pins");

		// Array-style constructor (AVR-friendly)
		ParallelBus(const uint8_t* pins, uint8_t width) {
			_width = (width > MaxWidth) ? MaxWidth : width;
			memcpy(_pins, pins, _width);
		}

		// C-style variadic constructor (AVR-compatible)
		ParallelBus(uint8_t firstPin, ...) {
			_pins[0] = firstPin;
			_width = 1;

			va_list args;
			va_start(args, firstPin);
			while (_width < MaxWidth) {
				int pin = va_arg(args, int);
				if (pin == -1) break;  // Terminate with -1
				_pins[_width++] = pin;
			}
			va_end(args);
		}
#ifndef __AVR__
		// Modern C++11 style constructor
		ParallelBus(std::initializer_list<uint8_t> pins) {
			_width = 0;
			for (uint8_t pin : pins) {
				if (_width >= MaxWidth) break;
				_pins[_width++] = pin;
			}
		}
#endif

		// Digital write a multi-bit bus
		friend void digitalWrite(ParallelBus &bus, uint16_t value) { bus.write(value); }
		// Digital read a multi-bit bus
		friend uint16_t digitalRead(ParallelBus &bus) { return bus.read(); }
		// Set the mode of all pins of a bus
		friend void pinMode(ParallelBus &bus, uint8_t mode) { bus.pinmode(mode); }

	private:
		void write(uint16_t value) {
			for (uint8_t i = 0; i < _width; i++) {
				::digitalWrite(_pins[i], (value >> i) & 0x01);
			}
		}

		uint16_t read() {
			uint16_t val = 0;
			for (uint8_t i = 0; i < _width; i++) {
				val |= (::digitalRead(_pins[i]) << i);
			}
			return val;
		}

		void pinmode(uint8_t mode) {
			for (uint8_t i = 0; i < _width; i++) {
				::pinMode(_pins[i], mode);
			}
		}

		uint8_t _pins[MaxWidth];
		uint8_t _width;
};

using ParallelBus8 = ParallelBus<8>;
using ParallelBus16 = ParallelBus<16>;
