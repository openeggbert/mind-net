const _state = {
    entities: [],
    entityLabels: {},
    actions: [],
    entitySchemas: {},

    selectedEntity: null,
    selectedAction: null,
    selectedActionId: null,

    currentPage: 1,
    pageSize: 10,
    totalPages: 1,
};

// getters
export const getEntities = () => _state.entities;
export const getEntityLabels = () => _state.entityLabels;
export const getActions = () => _state.actions;
export const getEntitySchemas = () => _state.entitySchemas;
export const getSelectedEntity = () => _state.selectedEntity;
export const getSelectedAction = () => _state.selectedAction;
export const getSelectedActionId = () => _state.selectedActionId;
export const getCurrentPage = () => _state.currentPage;
export const getPageSize = () => _state.pageSize;
export const getTotalPages = () => _state.totalPages;

// setters
export const setEntities = (value) => { _state.entities = value; };
export const setEntityLabels = (value) => { _state.entityLabels = value; };
export const setActions = (value) => { _state.actions = value; };
export const setEntitySchemas = (value) => { _state.entitySchemas = value; };
export const setSelectedEntity = (value) => { _state.selectedEntity = value; };
export const setSelectedAction = (value) => { _state.selectedAction = value; };
export const setSelectedActionId = (value) => { _state.selectedActionId = value; };
export const setCurrentPage = (value) => { _state.currentPage = value; };
export const setPageSize = (value) => { _state.pageSize = value; };
export const setTotalPages = (value) => { _state.totalPages = value; };


export const mainEntities = ['map', 'note', 'tag', 'property'];
export const linkEntities = ['reference', 'link'];
export const reviewEntities = ['review', 'sm2_state'];
export const collaborationEntities = ['user', 'team', 'team_member', 'message', 'discussion', 'comment'];
export const suggestionEntities = ['suggestion', 'suggestion_review'];
export const notMainEntities = [linkEntities, reviewEntities, collaborationEntities, suggestionEntities];
//
export const actionLabels = {
    list: '📋 List',
    create: '➕ Create',
    read: '📖 Read',
    update: '✏️ Update',
    delete: '🗑️ Delete',
    explore: '🗺️ Explore'
};

