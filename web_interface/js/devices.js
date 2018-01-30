var IP_RANGE = '192.168.43.1-254';
var IP_LIST = ['192.168.43.2'];

function mapObject(object, callback) {
  return Object.keys(object).map(function (key) {
    return callback(key, object[key]);
  });
};
function isFunction(functionToCheck) {
 var getType = {};
 return functionToCheck && getType.toString.call(functionToCheck) === '[object Function]';
}

function pakpos(nip = IP_LIST[0], data = {cmd: 'get', value: 'devices_list'}, success = function(response){return response}, error = function(xhr, status){return status}){
  $.ajax({
      url: 'http://'+nip+'/cgi',
      type: 'POST',
      // This is the important part
      xhrFields: {
        withCredentials: false
      },
      // This is the important part
      data: data,
      success: success,
      error: error,
      timeout: 1000
  });
}

class Main extends React.Component{
  constructor(props){
    super(props);
    this.state = {
      deviceList: [],
      devCount: 0,
      devActive: 0,
      devUsage: 0,
      xhr_log: ''
    }
    this.getDeviceList = this.getDeviceList.bind(this);
    this.scanDevice = this.scanDevice.bind(this);
    this.testIp = this.testIp.bind(this);
    this.mock = this.mock.bind(this);
    this.setLog = this.setLog.bind(this);
  }
  setLog(log){
    this.setState({xhr_log: log});
  }
  testIp(IP){
    this.setState({xhr_log: 'Mencoba mengontak ' + IP});
    pakpos(IP, {cmd: 'get', value: '{"func": "index"}'}, 
    function(response){
      IP_LIST.push(IP);
      this.setState({xhr_log: IP + ' ditambahkan ke daftar perangkat.'});
      this.setState({xhr_log: "Terdapat " + IP_LIST.length + " Node aktif dalam jaringan ini. Terakhir diuji: " + IP + " - " + "OK!"});
    }.bind(this), 
    function(xhr, status){
      this.setState({xhr_log: "Terdapat " + IP_LIST.length + " Node aktif dalam jaringan ini. Terakhir diuji: " + IP + " - " + status});
    }.bind(this));
  }
  scanDevice(){
    IP_LIST = [];
    if(IP_RANGE.indexOf('-') > -1){
      var ip_range_array = IP_RANGE.split('-');
      var start = ip_range_array[0];
      var end = parseInt(ip_range_array[1]);
      if(end == NaN || end > 254 || end < 1 || start.split(".").length < 4){
        this.setState({xhr_log: end + ' ' + 'IP awal/akhir tidak valid!'});
      }
      else{
        var ip_prefix = start.split(".");
        var IP_LIST_TEMP = [];
        for(var i = parseInt(start.split(".")[3]); i <= end; i++){
          var curr_ip = ip_prefix[0] + '.' + ip_prefix[1] + '.' + ip_prefix[2] + '.' + i;
          IP_LIST_TEMP.push(curr_ip);
        }
        
        var counter = 1;
        IP_LIST_TEMP.forEach(function(IP){
          setTimeout(function(){
            this.testIp(IP);
          }.bind(this),100*counter);
          counter++;
        }.bind(this));
      }
    }
    else{
      this.setState({xhr_log: IP_RANGE + ' ' + 'IP Range tidak valid!'});
      return false;
    }
  }
  mock(IP, id){
        var arrayvar = this.state.deviceList;
        pakpos(IP, {cmd: 'get', value: '{"func": "devices_list", "id": '+id+'}'}, function(response){
            response.usage = response.uptime/1000/60/60 * response.watt / 1000 * 1300;
            arrayvar[response.ip + '-' + response.id] = response;
            this.setState({ deviceList: arrayvar });
        }.bind(this));
  }
  getDeviceList(){
    var active = 0;
    var usage = 0;
    var devCount = 0;
    IP_LIST.forEach(function(IP) {
      pakpos(IP, {cmd: 'get', value: '{"func": "index"}'}, function(response){
        devCount = parseInt(devCount) + parseInt(response);
        for(var l = 1; l <= parseInt(response); l++){
            
                  (function(l) {
                     setTimeout(function(){
                        pakpos(IP, {cmd: 'get', value: '{"func": "devices_list", "id": '+l+'}'}, function(response){
                            var arrayvar = this.x.state.deviceList;
                            response.usage = response.uptime/1000/60/60 * response.watt / 1000 * 1300;
                            arrayvar[response.ip + '-' + response.id] = response;
                            this.x.setState({ deviceList: arrayvar });
                        }.bind(this));
                    }.bind({x:this, l: l}),180 * l);
                  }).bind(this)(l)
        }
        this.setState({xhr_log: IP + ' <---> ' + 'OK!'});
        this.setState({devCount: devCount, devActive: active, devUsage: Math.round(usage)});
        }.bind(this), 
        function(xhr, status){
          this.setState({xhr_log: IP + ' <---> ' + status});
        }.bind(this)
      );    
    }.bind(this));
    mapObject(this.state.deviceList, function(key, object){
        if(object.state){
            active++;
        }
        usage += object.usage;
      }
    );
  }
  render(){
    return(
      <div>
      <div className="top-log">{this.state.xhr_log}</div>
      <div className="container">
        <Header log={this.setLog} scan={this.scanDevice} refresh={this.getDeviceList} devCount={this.state.devCount} devActive={this.state.devActive} devUsage={this.state.devUsage}/>
        <Content deviceList={this.state.deviceList} refresh={this.mock}/>
        <div className="footet-cr">&copy; 2017 <a href="https://tutorkeren.com">Narin Laboratory</a></div>
      </div>
      </div>
    );
  }
}

class Header extends React.Component{
  constructor(props){
    super(props);
    this.state = {
        ip: IP_RANGE
    }
    this.refresh = this.refresh.bind(this);
    this.scan = this.scan.bind(this);
    this.changeIp = this.changeIp.bind(this);
    this.changeRange = this.changeRange.bind(this);
  }
  changeRange(){
    if(IP_RANGE == '192.168.43.1-254'){
      IP_RANGE = '172.20.10.2-14';
      IP_LIST = ['172.20.10.2'];
      this.props.log("IP Range diganti ke APPLE");
    }
    else{
      IP_RANGE = '192.168.43.1-254';
      IP_LIST = ['192.168.43.2'];
      this.props.log("IP Range diganti ke ANDROID");
    }
    this.setState({ip: IP_RANGE});
  }
  refresh(){
    this.props.refresh();
  }
  scan(){
    this.props.scan();
  }
  changeIp(event){
    this.setState({ip: event.target.value});
    IP_RANGE = event.target.value;
  }
  render(){
    var usage = this.props.devUsage;
    if(this.props.devUsage > 1000){
      usage = Math.round(usage/1000) + 'K';
    }
    return(
      <div id="header" className="row">
        <div className="col-xs-4">
          <div className="devices-stat">
            {this.props.devActive}/{this.props.devCount}
          </div>
        </div>
        <div className="col-xs-4">
          <div className="logo">
            <img src="img/narin_laboratory.png" alt="Narin Laboratory" className="img-responsive margin-center" onClick={this.changeRange}/>
          </div>
        </div>
        <div className="col-xs-4">
          <div className="devices-stat">
            {usage}
          </div>
        </div>
        <div id="top-menu" className="input-group">
          <span className="input-group-btn">
            <button onClick={this.scan} className="btn btn-default" type="button"><span className="glyphicon glyphicon-search" aria-hidden="true"></span></button>
          </span>
          <input type="text" name="ip" id="ip" className="form-control" placeholder={this.state.ip} onChange={this.changeIp}/>
          <span className="input-group-btn">
            <button className="btn btn-default" onClick={this.refresh}><span className="glyphicon glyphicon-refresh" aria-hidden="true"></span></button>
          </span>
        </div>
      </div>
    );
  }
}

class Content extends React.Component{
  constructor(props){
    super(props);
    this.state = {
      
    }
  }
  render(){
    var refresh = this.props.refresh;
    var render = mapObject(this.props.deviceList, function(key, objectKey){
        return(
          <Card key={key} device={objectKey} refresh={refresh}/>  
        );
    });
    return(
      <div className="device-list">
        {render}
      </div>
    );
  }
}

class Card extends React.Component{
  constructor(props){
    super(props);
    this.btnHandleClick = this.btnHandleClick.bind(this);
    this.state = {
        state: this.props.device.state
    }
  }
  getIcon(model){
    if(model == 'RELAY'){
      return 'img/relay.jpg';
    }
    else if(model == 'DS18B20'){
      return 'img/ds18b20.jpg'
    }
    else if(model == 'PIR'){
      return 'img/pir.jpg'
    }
    else if(model == 'STARTER'){
      return 'img/starter.jpg'
    }
    else{
      return 'img/narin_laboratory.png';
    }
  }
  btnHandleClick(e){
      this.setState({state: !this.state.state});
      if(this.props.device.model == "STARTER"){
          pakpos(this.props.device.ip,{cmd: 'set', value: '{"func": "control_relay", "id": '+this.props.device.id+', "value": 1}'}, this.props.refresh(this.props.device.ip, this.props.device.id));
      }
      else {
          pakpos(this.props.device.ip,{cmd: 'set', value: '{"func": "control_relay", "id": '+this.props.device.id+', "value": '+!this.state.state+'}'}, this.props.refresh(this.props.device.ip, this.props.device.id));
      }
  }
  getToggleClass(state){
      if(state == 0){
          return 'off';
      }
      else if(state == 1){
          return 'on';
      }
  }
  render(){
    //console.log(this.props);
    var usage = Math.round(this.props.device.uptime/1000/60/60 * this.props.device.watt / 1000 * 1300);
    if(usage > 1000){
      usage = Math.round(usage/1000) + 'K';
    }
    var uptime = Math.round(this.props.device.uptime/1000/60/60);
    var button = '';
    if(this.props.device.model == "STARTER"){
        button = <button className={this.getToggleClass(this.state.state)} onClick={this.btnHandleClick}><span>S</span></button>
    }
    else {
        button =  <button className={this.getToggleClass(this.state.state)} onClick={this.btnHandleClick}><span>{this.getToggleClass(this.state.state)}</span></button>;
    }
    return(
      <div className="row">
        <div className="col-xs-3"><img src={this.getIcon(this.props.device.model)} alt={this.props.device.model} className="img-responsive"/></div>
        <div className="col-xs-6">
          <h5>{this.props.device.name + ' @' + this.props.device.loc}</h5>
          <ul className="list-inline small-info">
            <li>Rp. {usage}</li>
            <li><span className="glyphicon glyphicon-time" aria-hidden="true"></span> {uptime}</li>
            <li><span className="glyphicon glyphicon-signal" aria-hidden="true"></span> {this.props.device.rssi}</li>
            <li>IP. {this.props.device.ip}</li>
          </ul>
        </div>
        <div className="col-xs-3">
         {button}
          
        </div>
      </div>
    );
  }
}

ReactDOM.render(
  <Main />,
  document.getElementById("content")
);