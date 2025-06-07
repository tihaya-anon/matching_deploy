// Decimal.h
#ifndef MATCH_DECIMAL_H
#define MATCH_DECIMAL_H

#include <cstdint>
#include <string>

class Decimal {
private:
    int64_t raw_value;
    uint8_t scale;


    static int64_t power10(uint8_t exp);


public:
    static Decimal ZERO;
    static Decimal ONE;

    Decimal();

    Decimal(int64_t raw, uint8_t scale);

    static Decimal fromParts(int64_t int_part, int64_t frac_part, uint8_t scale);

    static Decimal fromStr(const std::string &str);

    [[nodiscard]] std::string toStr() const;

    [[nodiscard]] Decimal clone() const;

    [[nodiscard]] int64_t getRaw() const;

    [[nodiscard]] uint8_t getScale() const;

    [[nodiscard]] Decimal rescale(uint8_t new_scale) const;


    Decimal operator-(const Decimal &other) const;


    bool operator>(const Decimal &other) const;

    bool operator<(const Decimal &other) const;


    bool operator==(const Decimal &other) const;

};

#endif // MATCH_DECIMAL_H
