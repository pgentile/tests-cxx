#ifndef MODEL_CLICHE_CLIENT_HPP
#define MODEL_CLICHE_CLIENT_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "util/Optional.hpp"
#include "date/LocalDateTime.hpp"
#include "model/cliche/Compte.hpp"
#include "model/data/ClientData.hpp"


namespace model {
namespace cliche {

class Client final
{
    
public:
    
    Client(std::shared_ptr<model::data::ClientData> const& data, date::LocalDateTime const& date);
    
    uint64_t getId() const {
        return _data->getId();
    }
    
    std::string const& getIdFonc() const {
        return _data->getIdFonc();
    }
    
    Compte& getCompte() {
        return *_compte;
    }
    
    Compte const& getCompte() const {
        return *_compte;
    }

private:
    
    std::shared_ptr<model::data::ClientData> _data;
    
    util::Optional<Compte> _compte;

};

} // namespace cliche
} // namespace model

#endif
