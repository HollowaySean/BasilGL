#ifndef SRC_DATA_IDATAMODEL_HPP_
#define SRC_DATA_IDATAMODEL_HPP_

namespace basil {

/** @brief Interface containing data which is passed in PubSub data model. */
class IDataModel {
 public:
    /** @returns UID of DataModel */
    unsigned int getID() const { return ID; }

 protected:
    IDataModel() : ID(nextID++) {}

 private:
    const unsigned int ID;
    static inline unsigned int nextID = 1;
};

}  // namespace basil

#endif  // SRC_DATA_IDATAMODEL_HPP_
