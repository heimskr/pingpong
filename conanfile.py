from conans import ConanFile, tools, Meson
import os

class PingPongBase(ConanFile):
	default_options = \
		"boost:shared=False"

	exports_sources = "include/*"

	generators = "pkg_config"

	requires = \
		"Poco/1.9.0@pocoproject/stable"

	settings = \
		"os", \
		"compiler", \
		"build_type"

	def build(self):
		meson = Meson(self)
		meson.configure(build_folder="./build", pkg_config_paths="./build")
		meson.build()
