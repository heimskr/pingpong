from conans import ConanFile, tools, Meson
import os

class PingPongBase(ConanFile):
	name = "pingpong"
	version = "0.0"
	generators = "pkg_config"
	requires = "Poco/1.9.0@pocoproject/stable"
	settings = "os", "compiler", "build_type", "arch"

	def build(self):
		meson = Meson(self)
		# meson.configure(build_folder="./build", pkg_config_paths="./build")
		meson.configure(source_folder="%s/src" % self.source_folder, build_folder="%s/builddir" % self.source_folder)
		# meson.configure(source_folder="%s/src" % self.source_folder, build_folder="builddir")
		meson.build()
