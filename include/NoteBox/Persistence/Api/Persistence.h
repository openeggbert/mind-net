//
// Created by robertvokac on 8/6/25.
//

#ifndef NOTEBOX_H
#define NOTEBOX_H


namespace NoteBox::Manager {
    class Persistence {
    public:
        NoteManager notes;
        TagManager tags;
        CollectionManager collections;
        ReferenceManager references;

        NoteBox(std::shared_ptr<Persistence> db)
            : notes(db), tags(db), collections(db), references(db) {}
    };

}


#endif //NOTEBOX_H
