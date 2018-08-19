#include "ML_modules.hpp"

#include "dsp/digital.hpp"

struct OctaTimes : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		IN5_INPUT,
		IN6_INPUT,
		IN7_INPUT,
		IN8_INPUT,
		IN_B_1_INPUT,
		IN_B_2_INPUT,
		IN_B_3_INPUT,
		IN_B_4_INPUT,
		IN_B_5_INPUT,
		IN_B_6_INPUT,
		IN_B_7_INPUT,
		IN_B_8_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		OUT3_OUTPUT,
		OUT4_OUTPUT,
		OUT5_OUTPUT,
		OUT6_OUTPUT,
		OUT7_OUTPUT,
		OUT8_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	SchmittTrigger trigger[8];
	float out[8];


	OctaTimes() : Module( NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS ) { reset(); };

	void step() override;

	void reset() override {
		for(int i=0; i<8; i++) out[i] = 0.0;
	};

};



void OctaTimes::step() {

	float in_A[8], in_B[8];

	float random = 0;




	in_A[0] = inputs[IN1_INPUT].normalize(random);
	for(int i=1; i<8; i++) in_A[i] = inputs[IN1_INPUT+i].normalize(in_A[i-1]);

	in_B[0] = inputs[IN_B_1_INPUT].normalize(random);
	for(int i=1; i<8; i++) in_B[i] = inputs[IN_B_1_INPUT+i].normalize(in_B[i-1]);

	for(int i=0; i<8; i++) outputs[OUT1_OUTPUT+i].value = in_A[i] * in_B[i] / 10.0f;

};



struct OctaTimesWidget : ModuleWidget {
	OctaTimesWidget(OctaTimes *module);
};

OctaTimesWidget::OctaTimesWidget(OctaTimes *module) : ModuleWidget(module) {

	box.size = Vec(15*8, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin,"res/OctaTimes.svg")));

		addChild(panel);
	}

	addChild(Widget::create<MLScrew>(Vec(15, 0)));
	addChild(Widget::create<MLScrew>(Vec(box.size.x-30, 0)));
	addChild(Widget::create<MLScrew>(Vec(15, 365)));
	addChild(Widget::create<MLScrew>(Vec(box.size.x-30, 365)));




	const float offset_y = 60, delta_y = 32, row1=15, row2 = 48, row3 = 80;

	for( int i=0; i<8; i++) {
		addInput(Port::create<MLPort>(Vec(row1, offset_y + i*delta_y  ), Port::INPUT, module, OctaTimes::IN1_INPUT+i));
		addInput(Port::create<MLPort>(Vec(row2, offset_y + i*delta_y  ), Port::INPUT, module, OctaTimes::IN_B_1_INPUT+i));
		addOutput(Port::create<MLPort>(Vec(row3, offset_y + i*delta_y ), Port::OUTPUT, module, OctaTimes::OUT1_OUTPUT+i));
	};


}

Model *modelOctaTimes = Model::create<OctaTimes, OctaTimesWidget>("ML modules", "OctaTimes", "OctaTimes", UTILITY_TAG);