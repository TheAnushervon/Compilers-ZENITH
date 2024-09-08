{ pkgs }: {
	deps = [
   pkgs.q-text-as-data
		pkgs.clang
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}