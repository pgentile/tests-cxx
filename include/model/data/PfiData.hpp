#ifndef MODEL_DATA_PFI_HPP
#define MODEL_DATA_PFI_HPP

#include <stdint.h>
#include <string>
#include <vector>

#include "boost/shared_ptr.hpp"

#include "date/LocalDateTimeRange.hpp"
#include "model/data/ComptePayeurData.hpp"


namespace model {
namespace data {

class PfiData
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
    
    boost::shared_ptr<date::LocalDateTimeRange> getValidite() const {
        return _validite;
    }
    
    void setValidite(boost::shared_ptr<date::LocalDateTimeRange> validite) {
        _validite = validite;
    }
    
    std::vector<boost::shared_ptr<ComptePayeurData> >& getComptesPayeurs() {
        return _comptesPayeurs;
    }
    
    std::vector<boost::shared_ptr<ComptePayeurData> > const& getComptesPayeurs() const {
        return _comptesPayeurs;
    }
    
    void setComptesPayeurs(std::vector<boost::shared_ptr<ComptePayeurData> > const& comptesPayeurs) {
        _comptesPayeurs = comptesPayeurs;
    }

private:
    
    uint64_t _id;
    
    std::string _idFonc;
    
    boost::shared_ptr<date::LocalDateTimeRange> _validite;
    
    std::vector<boost::shared_ptr<ComptePayeurData> > _comptesPayeurs;
    
};

} // namespace data
} // namespace model

#endif
