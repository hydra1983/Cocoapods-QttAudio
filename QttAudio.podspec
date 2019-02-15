#
# Be sure to run `pod lib lint tpcrasheye.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
    s.name             = 'QttAudio'
    s.version          = '2019.02.15'
    s.summary          = 'QttAudio'
  
  # This description is used to generate tags and improve search results.
  #   * Think: What does it do? Why did you write it? What is the focus?
  #   * Try to keep it short, snappy and to the point.
  #   * Write the description between the DESC delimiters below.
  #   * Finally, don't worry about the indent, CocoaPods strips it!
  
    s.homepage         = 'https://gitee.com/qttaudio/iOS/tree/master'
    s.author           = { 'hydra1983' => 'hydra1983@gmail.com' }
    s.source           = { :git => 'http://gitlab.imtv.tech:1080/repos/cocoapods/QttAudio.git', :tag => s.version.to_s }
    s.source_files =  'QttAudio/Headers/*.h'
    s.preserve_paths = 'QttAudio/libQttAudio.a'
    s.vendored_libraries = "QttAudio/libQttAudio.a"
    s.ios.deployment_target = '9.0'
    s.requires_arc = true
    s.xcconfig  =  { 
      'LIBRARY_SEARCH_PATHS' => '"$(PODS_ROOT)/QttAudio"',
      'HEADER_SEARCH_PATHS' => '"${PODS_ROOT}/Headers/QttAudio"' }
    s.license          = { :type => 'MIT', :file => 'LICENSE' }
    s.libraries = ['z', 'c++']
  end
  