#include <cmath>
#include <iostream>

#include <lv2.h>


struct dynamics {
    float *ports[5];
    float abs1;
    float abs2;
    float samplerate;
};

static LV2_Handle instantiate(const LV2_Descriptor *descriptor, double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    dynamics *instance = new dynamics;
    instance->samplerate = sample_rate;
    instance->abs1 = 0;
    instance->abs2 = 0;
    return (LV2_Handle)(instance);
}

static void cleanup(LV2_Handle instance)
{
    dynamics *tinstance = (dynamics*)instance;
    delete tinstance;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    ((dynamics*)instance)->ports[port] = (float*)data_location;
}

static void run(LV2_Handle instance, uint32_t sample_count)
{
    dynamics *tinstance = (dynamics*)(instance);

    const float t1 = tinstance->ports[2][0] / 1000.0f;
    const float t2 = tinstance->ports[3][0] / 1000.0f;
    const float strength = tinstance->ports[4][0];

    const float a1 = 1.0f - expf((-1.0f/tinstance->samplerate) / t1);
    const float a2 = 1.0f - expf((-1.0f/tinstance->samplerate) / t2);

    for(uint32_t sample_index = 0; sample_index < sample_count; ++sample_index)
    {
        tinstance->abs1 = a1 * fabs(tinstance->ports[0][sample_index]) + (1.0f - a1) * tinstance->abs1;
        tinstance->abs2 = a2 * fabs(tinstance->ports[0][sample_index]) + (1.0f - a2) * tinstance->abs2;

        const float r = tinstance->abs1 / tinstance->abs2;

        tinstance->ports[1][sample_index] = powf(1.0f / r, tinstance->ports[4][0]) * tinstance->ports[0][sample_index];
    }
}

static const LV2_Descriptor descriptor = {
    "http://fps.io/plugins/relative_compressor",
    instantiate,
    connect_port,
    nullptr, // activate
    run,
    nullptr, // deactivate
    cleanup,
    nullptr // extension_data
};

LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor (uint32_t index)
{
    if (0 == index) return &descriptor;
    else return nullptr;
}
