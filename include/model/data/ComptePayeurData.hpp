#ifndef MODEL_DATA_COMPTE_PAYEUR_HPP
#define MODEL_DATA_COMPTE_PAYEUR_HPP

#include <cstdint>
#include <string>
#include <memory>

#include "util/Optional.hpp"
#include "date/LocalDateTimeRange.hpp"


namespace model {
namespace data {

class ComptePayeurData final
{

public:

    uint64_t getId() const {
        return _id;
    }
    
    void setId(uint64_t id) {
        _id = id;
    }
    
    std::string const& getIdFonc() const {
        return _idFonc;
    }
    
    void setIdFonc(std::string const& idFonc) {
        _idFonc = idFonc;
    }
    
    date::LocalDateTimeRange const& getValidite() const {
        return *_validite;
    }
    
    void setValidite(date::LocalDateTimeRange const& validite) {
        _validite = validite;
    }

private:
    
    uint64_t _id;
    
    std::string _idFonc;
    
    util::Optional<date::LocalDateTimeRange> _validite;
    
};

} // namespace data
} // namespace model

#endif
