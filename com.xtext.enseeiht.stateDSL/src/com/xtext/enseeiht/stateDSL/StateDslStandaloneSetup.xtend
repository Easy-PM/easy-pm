/*
 * generated by Xtext 2.9.1
 */
package com.xtext.enseeiht.stateDSL


/**
 * Initialization support for running Xtext languages without Equinox extension registry.
 */
class StateDslStandaloneSetup extends StateDslStandaloneSetupGenerated {

	def static void doSetup() {
		new StateDslStandaloneSetup().createInjectorAndDoEMFRegistration()
	}
}
