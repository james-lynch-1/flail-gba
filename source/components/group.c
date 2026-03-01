#include "component.h"

void addComponentGroup(s16 entId, u16 flags, s16* memberIds, void (*onCollect)(struct MemberComponent_*), int numMembers) {
    GroupComponent g = { {entId, 0}, {}, NULL, 0, numMembers };
    if (memberIds && (numMembers > 0))
        for (int i = 0; i < numMembers; i++)
            g.memberIds[i] = memberIds[i];
    g.onCollect = onCollect;
    addComponentCustom(&g, COMP_GROUP);
}
