// Decimal.cpp
#include "decimal.h"
#include <stdexcept>
#include <cstdlib>

int64_t safe_str_to_int64(const std::string &int_part_str) {
    const char *str = int_part_str.c_str();
    char *end_ptr = nullptr;
    errno = 0;

    int64_t value = std::strtoll(str, &end_ptr, 10);
    if (errno == ERANGE) {
        throw std::out_of_range("Integer value out of range");
    }
    if (end_ptr == str || *end_ptr != '\0') {
        throw std::invalid_argument("Invalid integer string: '" + int_part_str + "'");
    }
    return value;
}

Decimal Decimal::ZERO = Decimal(0, 0);

int64_t Decimal::power10(uint8_t exp) {
    int64_t result = 1;
    while (exp--) result *= 10;
    return result;
}

Decimal::Decimal() : raw_value(0), scale(8) {}

Decimal::Decimal(int64_t raw, uint8_t scale) : raw_value(raw), scale(scale) {}

Decimal Decimal::fromParts(int64_t int_part, int64_t frac_part, uint8_t scale) {
    int64_t factor = power10(scale);
    return {int_part * factor + frac_part, scale};
}

int64_t Decimal::getRaw() const { return raw_value; }

uint8_t Decimal::getScale() const { return scale; }


Decimal Decimal::operator-(const Decimal &other) const {
    if (scale != other.scale)
        throw std::invalid_argument("Mismatched scales in subtraction");
    return {raw_value - other.raw_value, scale};
}

Decimal Decimal::rescale(uint8_t new_scale) const {
    if (new_scale == scale) return *this;

    int64_t factor_diff = power10(std::abs(new_scale - scale));

    if (new_scale > scale) {
        return {raw_value * factor_diff, new_scale};
    } else {
        throw std::invalid_argument("Loss accuracy");
    }
}

bool Decimal::operator>(const Decimal &other) const {
    if (scale == other.scale)return raw_value > other.raw_value;
    if (scale > other.scale) {
        return raw_value * power10(scale - other.scale) > other.raw_value;
    } else {
        return raw_value > other.raw_value * power10(other.scale - scale);
    }
}

bool Decimal::operator<(const Decimal &other) const {
    if (scale == other.scale)return raw_value < other.raw_value;
    if (scale > other.scale) {
        return raw_value * power10(scale - other.scale) < other.raw_value;
    } else {
        return raw_value < other.raw_value * power10(other.scale - scale);
    }
}

bool Decimal::operator==(const Decimal &other) const {
    if (scale == other.scale)return raw_value == other.raw_value;
    if (scale > other.scale) {
        return raw_value * power10(scale - other.scale) == other.raw_value;
    } else {
        return raw_value == other.raw_value * power10(other.scale - scale);
    }
}


Decimal Decimal::clone() const {
    return {raw_value, scale};
}

Decimal Decimal::fromStr(const std::string &str) {
    size_t dot_pos = str.find('.');
    uint8_t scale = dot_pos == std::string::npos ? 0 : str.length() - dot_pos - 1;
    if (dot_pos == std::string::npos) {
        int64_t int_part = safe_str_to_int64(str);
        return {int_part * power10(scale), scale};
    }

    std::string int_part_str = str.substr(0, dot_pos);
    std::string frac_part_str = str.substr(dot_pos + 1);

    if (frac_part_str.length() > scale) {
        frac_part_str = frac_part_str.substr(0, scale);
    } else {
        frac_part_str += std::string(scale - frac_part_str.length(), '0');
    }

    int64_t int_part = safe_str_to_int64(int_part_str);
    int64_t frac_part = safe_str_to_int64(frac_part_str);

    int64_t factor = power10(scale);
    return {int_part * factor + frac_part, scale};
}

std::string Decimal::toStr() const {
    int64_t factor = power10(scale);
    bool is_negative = raw_value < 0;

    int64_t abs_value = is_negative ? -raw_value : raw_value;

    int64_t int_part = abs_value / factor;
    int64_t frac_part = abs_value % factor;

    std::string result;

    if (is_negative) {
        result += '-';
    }

    result += std::to_string(int_part);

    if (frac_part != 0) {
        result += '.';
        std::string frac_str = std::to_string(frac_part);
        result += std::string(scale - frac_str.length(), '0') + frac_str;
    }

    return result;
}



