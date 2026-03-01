#include "component.h"

void addComponentMember(s16 entId, u16 flags, int groupId) {
    MemberComponent m = { {entId, flags}, groupId };
    addComponentCustom(&m, COMP_MEMBER);
    GroupComponent* group = getComponent(groupId, COMP_GROUP);
    group->memberIds[group->numMembers++] = entId;
}
