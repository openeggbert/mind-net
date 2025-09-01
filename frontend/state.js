export let entities = [];
export let entityLabels = {};
export let actions = [];
export let entitySchemas = {};

export let selectedEntity = null;
export let selectedAction = null;
export let selectedActionId = null;

// Pagination state
export let currentPage = 1;
export let pageSize = 10;
export let totalPages = 1;

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