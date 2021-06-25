@echo ##please input the ecc filename which is saved##
@echo ##for example, test.ecc, you can input test.##
@set /p ecc_fn=
@echo ## your config file = %ecc_fn%.ecc##

@if exist config_file\%ecc_fn%.ecc goto finded
@goto unfind
:finded
	rem /*create tmp*/ 
	mkdir tmp
	rem /*call configGenerator.exe*/
	.\..\..\..\tools\ConfigGenerator\ConfigGenerator.exe config_file\%ecc_fn%_install\include\pkgconf\ tmp\
	
	cleartool checkout -nc .\..\..\..\usdk213f\case\include\cfg_name.h
	cp tmp\cfg_name.h .\..\..\..\..\usdk213f\case\include
	cleartool checkin -c "cfg_input" -ide .\..\..\..\usdk213f\case\include\cfg_name.h
	
	cleartool checkout -nc .\..\..\..\usdk213f\case\images\config.txt
	cleartool checkout -nc .\..\..\..\usdk213f\case\images\config.spec
	cp tmp\config.txt .\..\..\..\usdk213f\case\images
	cp tmp\config.spec .\..\..\..\usdk213f\case\images
	cleartool checkin -c "cfg_input" -ide .\..\..\..\usdk213f\case\images\config.txt
	cleartool checkin -c "cfg_input" -ide .\..\..\..\usdk213f\case\images\config.spec
	
	rem /*remove tmp */
	rmdir /s /q tmp
	@echo done!
	@goto end
:unfind
	@echo ##not find!##
	@goto end
:end
	@pause

