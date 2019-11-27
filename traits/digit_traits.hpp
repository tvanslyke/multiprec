#ifndef TIM_MULTIPREC_TRAITS_DIGIT_TRAITS_HPP
#define TIM_MULTIPREC_TRAITS_DIGIT_TRAITS_HPP

#include <type_traits>
#include <cstdint>

namespace tim::multiprec::traits {

template <class Digit, bool = std::is_integral_v<Digit> && std::is_unsigned_v<Digit>>
struct digit_traits;

template <class Digit>
struct digit_traits<Digit, true> {
	static constexpr const std::size_t bits = sizeof(Digit) * CHAR_BIT;
	static constexpr const Digit mask = ~static_cast<Digit>(0ul);
	static constexpr const Digit low_mask = mask >> (bits / 2ul);
	static constexpr const Digit high_mask = ~low_mask;
	static constexpr const bool has_wider_digit
		= sizeof(std::uintmax_t) > sizeof(Digit);
	static constexpr const bool has_double_width_digit
		= sizeof(unsigned short) >= sizeof(Digit) * 2ul
		|| sizeof(unsigned int) >= sizeof(Digit) * 2ul
		|| sizeof(unsigned long) >= sizeof(Digit) * 2ul
		|| sizeof(unsigned long long) >= sizeof(Digit) * 2ul
		|| sizeof(std::uintmax_t) >= sizeof(Digit) * 2ul;
	using double_width_digit_type = std::conditional_t<
		sizeof(unsigned short) >= sizeof(Digit) * 2ul,
		unsigned short,
		std::conditional_t<
			sizeof(unsigned int) >= sizeof(Digit) * 2ul,
			unsigned int,
			std::conditional_t<
				sizeof(unsigned long) >= sizeof(Digit) * 2ul,
				unsigned long,
				std::conditional_t<
					sizeof(unsigned long long) >= sizeof(Digit) * 2ul,
					unsigned long long,
					std::conditional_t<
						sizeof(std::uintmax_t) >= sizeof(Digit) * 2ul,
						std::uintmax_t,
						void
					>
				>
			>
		>
	>;
	using wider_digit_type = std::conditional_t<
		sizeof(unsigned short) > sizeof(Digit),
		unsigned short,
		std::conditional_t<
			sizeof(unsigned int) > sizeof(Digit),
			unsigned int,
			std::conditional_t<
				sizeof(unsigned long) > sizeof(Digit),
				unsigned long,
				std::conditional_t<
					sizeof(unsigned long long) > sizeof(Digit),
					unsigned long long,
					std::conditional_t<
						sizeof(std::uintmax_t) > sizeof(Digit),
						std::uintmax_t,
						void
					>
				>
			>
		>
	>;
};

} /* namespace tim::multiprec::traits */

#endif /* TIM_MULTIPREC_TRAITS_DIGIT_TRAITS_HPP */
