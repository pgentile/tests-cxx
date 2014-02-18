#ifndef MODEL_DATA_CLIENT_HPP
#define MODEL_DATA_CLIENT_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "util/Optional.hpp"
#include "date/LocalDateTimeRange.hpp"
#include "model/data/CompteData.hpp"


namespace model {
namespace data {

class ClientData final
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
    
    std::vector<std::shared_ptr<CompteData>>& getComptesPayeurs() {
        return _comptesPayeurs;
    }
    
    std::vector<std::shared_ptr<CompteData>> const& getComptesPayeurs() const {
        return _comptesPayeurs;
    }
    
    void setComptesPayeurs(std::vector<std::shared_ptr<CompteData>> const& comptesPayeurs) {
        _comptesPayeurs = comptesPayeurs;
    }

private:
    
    uint64_t _id;
    
    std::string _idFonc;
    
    util::Optional<date::LocalDateTimeRange> _validite;
    
    std::vector<std::shared_ptr<CompteData>> _comptesPayeurs;
    
};

} // namespace data
} // namespace model

#endif
