#ifndef SRC_DATA_IDATAMODEL_HPP_
#define SRC_DATA_IDATAMODEL_HPP_

namespace basil {

class IDataModel {
 public:
    unsigned int getID() const { return ID; }

 protected:
    IDataModel() : ID(nextID++) {}

 private:
    const unsigned int ID;
    static inline unsigned int nextID = 1;
};

}  // namespace basil

#endif  // SRC_DATA_IDATAMODEL_HPP_
