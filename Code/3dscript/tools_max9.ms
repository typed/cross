--功能：界面
--作者：龚晓剑
--时间：2013-12-3
--版本号：v0.1

c_sVersion = "v0.1";
c_sTitle = "gcross导出工具";

--获取时间
function getFramesSumTime nFrameCount = (
	local fFrameCount = nFrameCount as float;
	local fPer = 0.25;
	for i = 2 to timeConfiguration.playbackSpeed do fPer = fPer + fPer;
	return fFrameCount / frameRate / fPer;
)

--获取Skin修改器
function getSkin obj = (
	local s,i ;
	s = undefined ;
	if obj != undefined then
		for i in obj.modifiers do (
			if iskindof i Skin do
				s = i ;
		)
	return s;
)

--获取Physique修改器
function getPhysique obj = (
	local s,i ;
	s = undefined ;
	if obj != undefined then
		for i in obj.modifiers do (
			if iskindof i Physique do
				s = i ;
		)
	return s;
)

--获取法线
function getVertexNormalAccordingToSmoothingGroups selectedMesh faceIndex vertexIndex = (
	local normal;
	local faceSmoothingGroup = getFaceSmoothGroup selectedMesh faceIndex;
	if faceSmoothingGroup == 0 then (
		normal = getFaceNormal selectedMesh faceIndex
	) else (
		local vertexIndexes = getFace selectedMesh faceIndex;
		local vI = vertexIndexes[vertexIndex];
		local n = [0, 0, 0];
		for adjoiningFaceIndex in (meshop.getFacesUsingVert selectedMesh vI) do (
			local sg = getFaceSmoothGroup selectedMesh adjoiningFaceIndex;
			local and_sg = bit.and sg faceSmoothingGroup;
			if (and_sg != 0) then
				n += getFaceNormal selectedMesh adjoiningFaceIndex ;
		)
		normal = normalize n;
	)
	return normal;
)

--获取骨骼ID
function getBoneID name = (
	local i = 0;
	for obj in objects do (
		if (iskindof obj BoneObj) or (iskindof obj Dummy) then (
			if (name == obj.name) then return i;
			i = i + 1;
		)
	)
	return -1;
)

--获取顶点数据
function getVertexDatas selObj = (
	local sk = getSkin selObj;
	local phy = getPhysique selObj;
	if (sk != undefined) then (
		max modify mode;
		modPanel.setCurrentObject selObj.modifiers[#Skin];
	)
	if (phy != undefined) then (
		max modify mode;
		modPanel.setCurrentObject selObj.modifiers[#Physique];
	)
	local tmesh = snapshotAsMesh selObj;
	local data = #();
	local vertexData = #();
	local faceData = #();
	local vertexWeights = #();
	local iFace;
	local iVertexIndex;
	local numFaces = getNumFaces tmesh;
	for iFace = 1 to numFaces do (
		local faceVerticesIndices = getFace tmesh iFace;
		faceData[iFace] = faceVerticesIndices;
		for iFaceVi = 1 to 3 do (
		
			local vertexIndex = faceVerticesIndices[iFaceVi];
			if (vertexData[vertexIndex] == undefined) then (
				
				--顶点
				local vertexPosition = getVert tmesh vertexIndex;
				local vertTmp = copy vertexPosition;
				vertexPosition[2] = vertTmp[3];
				vertexPosition[3] = vertTmp[2];
				--print(vertexPosition as string);
				
				--法线
				local vertexNormal = getVertexNormalAccordingToSmoothingGroups tmesh iFace iFaceVi;
				vertexNormal = vertexNormal * -1;
				local nmalTmp = copy vertexNormal;
				vertexNormal[2] = nmalTmp[3];
				vertexNormal[3] = nmalTmp[2];
				--print(vertexNormal as string);
				
				--纹理坐标
				local UVcoords = #();
				local vertInds = meshOp.getMapFace tmesh 1 iFace;
				local vInd = vertInds[iFaceVi];
				local UVW = meshOp.getMapVert tmesh 1 vInd;
				append UVcoords UVW[1];
				append UVcoords (1 - UVW[2]);
				
				vertexData[vertexIndex] = #(vertexIndex, vertexPosition[1], vertexPosition[2], vertexPosition[3], vertexNormal[1], vertexNormal[2], vertexNormal[3], UVcoords[1], UVcoords[2]);
				
				--权重
				if (sk != undefined) then (
					local vertexWeightCount = skinOps.getVertexWeightCount sk vertexIndex;
					if (vertexWeightCount > 4) then (
						messageBox "顶点权重个数请不要超过4个！";
					)
					local k;
					for k = 1 to vertexWeightCount do (
						local bid = skinOps.getVertexWeightBoneID sk vertexIndex k;
						local bname = skinOps.getBoneName sk bid 1;
						local bone_id = getBoneID bname;
						local vertexWeight = skinOps.getVertexWeight sk vertexIndex k;
						local vm = #(vertexIndex, bone_id, vertexWeight);
						append vertexWeights vm;
					)
				)
				if (phy != undefined) then (
					local vertexWeightCount = physiqueOps.getVertexBoneCount selObj vertexIndex;
					if (vertexWeightCount > 4) then (
						messageBox "顶点权重个数请不要超过4个！";
					)
					local k;
					for k = 1 to vertexWeightCount do (
						local bone = physiqueOps.getVertexBone selObj vertexIndex k;
						local bone_id = getBoneID bone.bname;
						local vertexWeight = physiqueOps.getVertexWeight selObj vertexIndex k;
						local vm = #(vertexIndex, bone_id, vertexWeight);
						append vertexWeights vm;
					)
				)
			)
		)
	)
	
	--print(faceData as string);
	--print(vertexData as string);
	--print(vertexWeights as string);
	data[1] = vertexData;
	data[2] = faceData;
	data[3] = vertexWeights;
	return data;
)

--导出骨骼数据
function writeBones outFile = (
	format("\t<bones>\n") to:outFile;
	for obj in objects do (
		if (iskindof obj BoneObj) or (iskindof obj Dummy) then (
			local d3dm = obj.transform;
			local d3dmOffset = inverse(obj.transform);
			local pos = obj.position;
			local rot = obj.rotation;
			local scl = obj.scale;
			--print (d3dm as string);
			if (obj.parent != undefined) then (
				d3dm = d3dm * inverse(obj.parent.transform);
				rot = rot * inverse(obj.parent.rotation);
				pos = pos - obj.parent.position;
				format("\t\t<bone name=\"%\" parent=\"%\">\n") obj.name (getBoneID obj.parent.name) to:outFile;
			) else (
				format("\t\t<bone name=\"%\" parent=\"-1\">\n") obj.name to:outFile;
			)
			local axis = (rot as angleaxis).axis;
			local angle = degToRad((rot as angleaxis).angle);
			format("\t\t\t<d3dpos x=\"%\" y=\"%\" z=\"%\"/>\n") pos.x pos.z pos.y to:outFile;
			format("\t\t\t<d3drot angle=\"%\" x=\"%\" y=\"%\" z=\"%\"/>\n") angle axis.x axis.z axis.y to:outFile;
			format("\t\t\t<d3dscl x=\"%\" y=\"%\" z=\"%\"/>\n") scl.x scl.z scl.y to:outFile;
			format("\t\t\t<d3dmatrix m0=\"%\" m1=\"%\" m2=\"%\" m3=\"0\" m4=\"%\" m5=\"%\" m6=\"%\" m7=\"0\" m8=\"%\" m9=\"%\" m10=\"%\" m11=\"0\" m12=\"%\" m13=\"%\" m14=\"%\" m15=\"1\" />\n") d3dm.row1.x d3dm.row1.z d3dm.row1.y d3dm.row3.x d3dm.row3.z d3dm.row3.y d3dm.row2.x d3dm.row2.z d3dm.row2.y d3dm.row4.x d3dm.row4.z d3dm.row4.y to:outFile;
			format("\t\t\t<d3dmatrixOffset m0=\"%\" m1=\"%\" m2=\"%\" m3=\"0\" m4=\"%\" m5=\"%\" m6=\"%\" m7=\"0\" m8=\"%\" m9=\"%\" m10=\"%\" m11=\"0\" m12=\"%\" m13=\"%\" m14=\"%\" m15=\"1\" />\n") d3dmOffset.row1.x d3dmOffset.row1.z d3dmOffset.row1.y d3dmOffset.row3.x d3dmOffset.row3.z d3dmOffset.row3.y d3dmOffset.row2.x d3dmOffset.row2.z d3dmOffset.row2.y d3dmOffset.row4.x d3dmOffset.row4.z d3dmOffset.row4.y to:outFile;
			format("\t\t</bone>\n") to:outFile;
		)
	)
	format("\t</bones>\n") to:outFile;
)

--导出模型数据
function writeMesh selObj outFile = (
	local data = getVertexDatas selObj;
	--print(data as string);
	local v;
	local f;
	local w;
	format("\t<mesh>\n") to:outFile;
	for v in data[1] do format("\t\t<vertex id=\"%\" posx=\"%\" posy=\"%\" posz=\"%\" norx=\"%\" nory=\"%\" norz=\"%\" u=\"%\" v=\"%\"/>\n") (v[1]-1) v[2] v[3] v[4] v[5] v[6] v[7] v[8] v[9] to:outFile;
	for f in data[2] do format("\t\t<face v1=\"%\" v2=\"%\" v3=\"%\"/>\n") (f[1]-1) (f[3]-1) (f[2]-1) to:outFile;
	for w in data[3] do format("\t\t<vertex_weight vertex_id=\"%\" bone_id=\"%\" weight=\"%\"/>\n") (w[1]-1) w[2] w[3] to:outFile;
	format("\t</mesh>\n") to:outFile;
)

--导出材质数据
function writeMaterials mat outFile = (
	format("\t<materials>\n") to:outFile;
	format("\t</materials>\n") to:outFile;
)

rollout gcrossExportTools (c_sTitle + " - " + c_sVersion)
(
	label lbl01 "XML转换器" pos:[2,2] width:70 height:18
	editText editXMLconverter "" pos:[74,2] width:160 height:18
	button browseXMLconverter "..." pos:[236,2] width:30 height:18 toolTip:"选择exe文件"
	
	button btnExportSkeleton "导出骨架" pos:[2,22] width:200 height:18
	button btnExportMesh "导出选择的模型" pos:[2,42] width:200 height:18
	button btnExportAni "导出动作" pos:[2,62] width:200 height:18
	button btnExportAniTest "测试用!" pos:[2,82] width:200 height:18
	
	on browseXMLconverter pressed do (
		filename = getOpenFileName types:"Executables(*.exe)|*.exe|";
		if (filename != undefined) then (
			editXMLconverter.text = filename;
			xmlexe = editXMLconverter.text;
			setINISetting ((getDir #scripts) + "\\gcrossScript.ini") "Options" "XMLConverterExe" xmlexe;
		)
	)
	on gcrossExportOptions open do (
		xmlexe = getINISetting ((getDir #scripts) + "\\gcrossScript.ini") "Options" "XMLConverterExe";
		editXMLconverter.text = xmlexe;
	)
	
	on btnExportSkeleton pressed do (
		local oldt = sliderTime;
		local ar = animationRange.start;
		sliderTime = ar;
		local aniName = (filterString maxFileName ".")[1];
		local sceneName = maxFilePath + aniName + ".skeleton.xml";
		local outFile = createfile sceneName;
		format("<skeleton>\n") to:outFile;
		writeBones outFile;
		format("</skeleton>\n") to:outFile;
		close outFile;
		DOSCommand (editXMLconverter.text + " -skeleton \"" + sceneName + "\"");
		sliderTime = oldt;
	)
	on btnExportMesh pressed do (
		local selObj = $;
		local oldt = sliderTime;
		local ar = animationRange.start;
		sliderTime = ar;
		if (selObj != undefined and (iskindof selObj GeometryClass)) then (
			local sceneName = maxFilePath + selObj.name + ".model.xml";
			local outFile = createfile sceneName;
			format("<model name=\"%\">\n") selObj.name to:outFile;
			writeMesh selObj outFile;
			writeMaterials selObj.material outFile;
			format("</model>\n") to:outFile;
			close outFile;
			DOSCommand (editXMLconverter.text + " -mesh \"" + sceneName + "\"");
		) else (
			messageBox "请选择一个有效的模型对象";
		)
		sliderTime = oldt;
	)
	on btnExportAni pressed do (
		local oldF = sliderTime;
		local aniName = (filterString maxFileName ".")[1];
		local sceneName = maxFilePath + aniName + ".ani.xml";
		local outFile = createfile sceneName;
		local sf = (animationRange.start as integer) / TicksPerFrame;
		local ef = (animationRange.end as integer) / TicksPerFrame;
		local smt = getFramesSumTime( ef - sf );
		format("<ani name=\"%\" sumtime=\"%\">\n") aniName smt to:outFile;
		local startTime = (animationRange.start as integer);
		for iFrame = sf to ef do (
			at time iFrame (
				local tm = getFramesSumTime(iFrame - sf);
				format("\t<key time=\"%\">\n") tm to:outFile;
				writeBones outFile;
				format("\t</key>\n") to:outFile;
			)
		)
		format("</ani>\n") to:outFile;
		close outFile;
		sliderTime = oldF;
		DOSCommand (editXMLconverter.text + " -ani \"" + sceneName + "\"");
	)
	on btnExportAniTest pressed do (
		--print(TicksPerFrame as string);
		--print((animationRange.end as integer) as string);
		--print(TicksPerFrame as string);
		local i = 0;
		for obj in objects do (
			if (iskindof obj BoneObj) or (iskindof obj Dummy) then (
				i = i + 1;
				--print(obj.position as string);
				--print((obj.rotation as angleaxis) as string);
				--print(obj.scale as string);
				local d3dm = obj.transform;
				local pos = obj.position;
				local rot = obj.rotation;
				if (obj.parent != undefined) then (
					d3dm = d3dm * inverse(obj.parent.transform);
					--rot = rot * inverse(obj.parent.rotation);
					pos = pos - obj.parent.position;
				)
				print(rot as string);
				print(d3dm.rotationpart as string);
			)
		)
	)
)

createdialog gcrossExportTools width:280 height:100;
registerViewWindow gcrossExportTools;