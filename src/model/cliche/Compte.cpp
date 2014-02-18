#include "model/cliche/Compte.hpp"

using namespace std;
using date::LocalDateTime;
using model::data::CompteData;


namespace model {
namespace cliche {

Compte::Compte(shared_ptr<CompteData> const& data):
    _data(data)
{
}

} // namespace cliche
} // namespace model
