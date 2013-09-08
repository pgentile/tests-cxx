#ifndef MODEL_DATA_COMPTEPAYEUR_HPP
#define MODEL_DATA_COMPTEPAYEUR_HPP

#include <cstdint>
#include <string>
#include <memory>

#include "date/LocalDateTimeRange.hpp"


namespace model {
namespace data {

class ComptePayeurData
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
    
    std::shared_ptr<date::LocalDateTimeRange> getValidite() const {
        return _validite;
    }
    
    void setValidite(std::shared_ptr<date::LocalDateTimeRange> validite) {
        _validite = validite;
    }

private:
    
    uint64_t _id;
    
    std::string _idFonc;
    
    std::shared_ptr<date::LocalDateTimeRange> _validite;
    
};

} // namespace data
} // namespace model

#endif
