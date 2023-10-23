/**
 * @file style.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Defines ANSI escape codes for text colors and styles.
 * @version 1.0
 * @date 2023-10-20
 *
 * ANSI escape sequences are a standard for in-band signaling to control cursor
 * location, color, font styling, and other options on video text terminals and
 * terminal emulators.
 * [Learn more](https://en.wikipedia.org/wiki/ANSI_escape_code)
 *
 * @copyright Copyright (c) 2023
 */

#ifndef STYLE_HPP_
#define STYLE_HPP_

namespace ext {
/**
 * @brief Foreground namespace
 *
 * This namespace defines ANSI Escape Codes for font coloring.
 */
namespace cfg {
inline constexpr short const none{-1};           ///< No foreground color
inline constexpr short const black{30};          ///< Black color
inline constexpr short const red{31};            ///< Red color
inline constexpr short const green{32};          ///< Green color
inline constexpr short const yellow{33};         ///< Yellow color
inline constexpr short const blue{34};           ///< Blue color
inline constexpr short const magenta{35};        ///< Magenta color
inline constexpr short const cyan{36};           ///< Cyan color
inline constexpr short const white{37};          ///< White color
inline constexpr short const bright_black{90};   ///< Bright black color
inline constexpr short const bright_red{91};     ///< Bright red color
inline constexpr short const bright_green{92};   ///< Bright green color
inline constexpr short const bright_yellow{93};  ///< Bright yellow color
inline constexpr short const bright_blue{94};    ///< Bright blue color
inline constexpr short const bright_magenta{95}; ///< Bright magenta color
inline constexpr short const bright_cyan{96};    ///< Bright cyan color
inline constexpr short const bright_white{97};   ///< Bright white color

inline constexpr short const list[] = {
    none,        black,        red,           green,       yellow,
    blue,        magenta,      cyan,          white,       bright_black,
    bright_red,  bright_green, bright_yellow, bright_blue, bright_magenta,
    bright_cyan, bright_white}; ///< List of foreground colors
} // namespace cfg

/**
 * @brief Background namespace
 *
 * This namespace defines ANSI Escape Codes for background coloring.
 */
namespace cbg {
inline constexpr short const none{-1};            ///< No color
inline constexpr short const black{40};           ///< Black
inline constexpr short const red{41};             ///< Red
inline constexpr short const green{42};           ///< Green
inline constexpr short const yellow{43};          ///< Yellow
inline constexpr short const blue{44};            ///< Blue
inline constexpr short const magenta{45};         ///< Magenta
inline constexpr short const cyan{46};            ///< Cyan
inline constexpr short const white{47};           ///< White
inline constexpr short const bright_black{100};   ///< Bright black
inline constexpr short const bright_red{101};     ///< Bright red
inline constexpr short const bright_green{102};   ///< Bright green
inline constexpr short const bright_yellow{103};  ///< Bright yellow
inline constexpr short const bright_blue{104};    ///< Bright blue
inline constexpr short const bright_magenta{105}; ///< Bright magenta
inline constexpr short const bright_cyan{106};    ///< Bright cyan
inline constexpr short const bright_white{107};   ///< Bright white

inline constexpr short const list[] = {
    none,        black,        red,           green,       yellow,
    blue,        magenta,      cyan,          white,       bright_black,
    bright_red,  bright_green, bright_yellow, bright_blue, bright_magenta,
    bright_cyan, bright_white}; ///< List of background colors
} // namespace cbg

/**
 * @brief Style namespace
 *
 * This namespace defines ANSI Escape Codes for font styling.
 */
namespace stl {
inline constexpr short const none{-1};              ///< No styling
inline constexpr short const regular{0};            ///< Regular
inline constexpr short const bold{1};               ///< Bold
inline constexpr short const faint{2};              ///< Faint
inline constexpr short const italic{3};             ///< Italic
inline constexpr short const underline{4};          ///< Underlined
inline constexpr short const reverse{7};            ///< Reverse
inline constexpr short const hide{8};               ///< Hidden
inline constexpr short const strike{9};             ///< Strikethrough
inline constexpr short const doubly_underlined{21}; ///< Doubly underlined

inline constexpr short const list[] = {
    none,   regular,          bold, faint, italic, underline, reverse, hide,
    strike, doubly_underlined}; ///< List of font styles
} // namespace stl
} // namespace ext

#endif // STYLE_HPP_
