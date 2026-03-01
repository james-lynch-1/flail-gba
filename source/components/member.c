#include "component.h"

MemberComponent* addComponentMember(s16 entId, u16 flags, int groupId) {
    MemberComponent m = { {entId, flags}, groupId };
    MemberComponent* mAddr = (MemberComponent*)addComponentCustom(&m, COMP_MEMBER);
    GroupComponent* group = getComponent(groupId, COMP_GROUP);
    if (group) group->memberIds[group->numMembers++] = entId;
    return mAddr;
}

void removeComponentMember(int entId) {
    MemberComponent* m = getComponent(entId, COMP_MEMBER);
    GroupComponent* g = getComponent(m->groupId, COMP_GROUP);
    if (g) {
        for (int i = 0; i < g->numMembers; i++) {
            if (g->memberIds[i] == entId) {
                g->memberIds[i] = g->memberIds[g->numMembers - 1];
                g->numMembers--;
            }
        }
    }
    removeComponent(entId, COMP_MEMBER);
}
