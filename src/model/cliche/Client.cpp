#include "model/cliche/Client.hpp"

#include "model/cliche/Compte.hpp"

using namespace std;
using date::LocalDateTime;
using model::data::CompteData;
using model::data::ClientData;


namespace model {
namespace cliche {

Client::Client(shared_ptr<ClientData> const& data, LocalDateTime const& date):
    _data(data)
{
    if (!data->getValidite().contains(date)) {
        throw logic_error("Client inactif a la date donnee");
    }
    
    for (auto const compteData: data->getComptesPayeurs()) {
        if (compteData->getValidite().contains(date)) {
            _compte = Compte(compteData);
            break;
        }
    }
    if (!_compte) {
        throw logic_error("Compte non trouve");
    }
}

} // namespace cliche
} // namespace model
