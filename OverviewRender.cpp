#include "OverviewRender.hpp"
#include <chrono>
#include <cmath>
#include <sstream>
#include <hyprland/src/render/Renderer.hpp>
#include <hyprland/src/render/pass/RectPassElement.hpp>

namespace OverviewRender {

void flushPass(PHLMONITOR monitor) {
    if (!monitor || g_pHyprRenderer->context().m_pass.empty())
        return;

    g_pHyprRenderer->context().m_pass.render(g_pHyprRenderer->context(), CRegion{CBox{{}, monitor->m_transformedSize}});
    g_pHyprRenderer->context().m_pass.clear();
}

void queueBlur(const CBox& box, int rounding, float roundingPower, float alpha, bool usePrecomputedBlur) {
    if (alpha <= 0.F)
        return;

    if (box.empty())
        return;

    CRectPassElement::SRectData data;
    data.box           = box;
    data.color         = CHyprColor{0.F, 0.F, 0.F, 0.F};
    data.round         = rounding;
    data.roundingPower = roundingPower;
    data.blur          = true;
    data.blurA         = alpha;
    data.xray          = usePrecomputedBlur;
    g_pHyprRenderer->context().m_pass.add(makeUnique<CRectPassElement>(data));
}

void renderBlur(PHLMONITOR monitor, const CBox& box, int rounding, float roundingPower, float alpha, bool usePrecomputedBlur) {
    if (!monitor)
        return;

    queueBlur(box, rounding, roundingPower, alpha, usePrecomputedBlur);
    flushPass(monitor);
}

}
