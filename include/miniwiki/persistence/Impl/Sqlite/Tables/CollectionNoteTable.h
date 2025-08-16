#ifndef COLLECTIONNOTETABLE_H
#define COLLECTIONNOTETABLE_H

namespace miniwiki::Persistence::Impl::Sqlite::Tables {
{
    struct CollectionNoteTable
    {
        CollectionNoteTable() = delete;

        CollectionNoteTable(const CollectionNoteTable&) = delete;

        CollectionNoteTable& operator=(const CollectionNoteTable&) = delete;

        static constexpr const char* TABLE_NAME = "COLLECTION_NOTE";

        static constexpr const char* COLLECTION_ID = "COLLECTION_ID";
        static constexpr const char* NOTE_ID = "NOTE_ID";
        static constexpr const char* ORDER_INDEX = "ORDER_INDEX";
    };
}
#endif // COLLECTIONNOTETABLE_H
