#include <cmath>
#include <iostream>

#include <lv2.h>


#include <rubberband/RubberBandStretcher.h>

struct stereo {
    RubberBand::RubberBandStretcher *stretcher;
    float buffer[1024];
    float *buffer_head;
    float *buffer_tail;
    float *ports[5];
};

static LV2_Handle instantiate(const LV2_Descriptor *descriptor, double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    stereo *instance = new stereo;
    instance->stretcher = new RubberBand::RubberBandStretcher(sample_rate, 1, RubberBand::RubberBandStretcher::OptionProcessRealTime | RubberBand::RubberBandStretcher::OptionEngineFaster, 2.0, 1.0);
    instance->stretcher->setMaxProcessSize(8);
    size_t startPad = instance->stretcher->getPreferredStartPad();
    std::cout << "startPad: " << startPad << "\n";
    for (size_t index = 0; index < startPad; ++index) {
        float *f = new float[1]; f[0] = 0;
        instance->stretcher->process(&f, 0, false);
        delete  []f;
    }
    
    std::cout << "available: " << instance->stretcher->available() << "\n";

    std::cout << "required: " << instance->stretcher->getSamplesRequired() << "\n";
    instance->buffer_head = instance->buffer;
    instance->buffer_tail = instance->buffer;
    return (LV2_Handle)(instance);
}

static void cleanup(LV2_Handle instance)
{
    stereo *tinstance = (stereo*)instance;
    delete tinstance->stretcher;
    delete tinstance;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    ((stereo*)instance)->ports[port] = (float*)data_location;
}

static void run(LV2_Handle instance, uint32_t sample_count)
{
    stereo *tinstance = (stereo*)(instance);

    const float max_delay = tinstance->ports[3][0];

    for(uint32_t sample_index = 0; sample_index < sample_count; ++sample_index)
    {
        // Just copy the left channel
        tinstance->ports[1][sample_index] = tinstance->ports[0][sample_index];

        
    }
}

static const LV2_Descriptor descriptor = {
    "http://fps.io/plugins/stereofication",
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
