import time, urequests, copy, re, json, traceback
KEY = "8d631bd83896441f8592036698471885"

# TODO:未做异常处理

def isonline():
    try:
        # requests.get("https://www.baidu.com").raise_for_status()
        return True
    except:
        return False


class WeatherDay:
    def __init__(self):
        self.temp_max = 0
        self.temp_min = 0
        self.text = ""
        self.icon_id = ""

        self.last_update = 0.0

    def set(self, inf):
        self.temp_max = inf["tempMax"]
        self.temp_min = inf["tempMin"]
        self.text = inf["textDay"]
        self.icon_id = inf["iconDay"]

        self.last_update = time.time()


class WeatherMore:
    def __init__(self, size=3):
        self._weathers = [WeatherDay() for i in range(size)]

        self.last_update = 0.0

    def __getitem__(self, item):
        return self._weathers[item]

    def set(self, inf):
        for i in range(len(self._weathers)):
            self._weathers[i].set(inf[i])
        self.last_update = time.time()


class Realtime:
    def __init__(self):
        self.temp = 0
        self.icon_id = 100
        self.text = ""

        self.last_update = 0.0

    def set(self, inf):
        self.temp = inf["temp"]
        self.icon_id = inf["icon"]
        self.text = inf["text"]

        self.last_update = time.time()


class Summary:
    def __init__(self):
        self.text = ""

        self.last_update = 0.0

    def set(self, inf):
        try:
            self.text = inf["summary"]
        except:
            self.text = "天气获取失败"
        self.last_update = time.time()


class Aqi:
    def __init__(self):
        self.aqi = 0

        self.last_update = 0.0

    def set(self, inf):
        self.aqi = inf["aqi"]

        self.last_update = time.time()

class QWeather():
    def __init__(self):
        self.last_update = 0
        self._city_name = None
        self._city_id = None
        self._city_lat = None
        self._city_lon = None
        self._inited = None

        self._summary = Summary()
        self._aqi = Aqi()
        self._weather_more = WeatherMore()
        self._realtime = Realtime()

    @property
    def city(self):
        return self._city_name

    def init(self):
        if isonline():
            try:
                #city_name = re.findall('省(.+?)市', requests.get("https://www.ip.cn/api/index?ip&type=0").text)[-1]
                city_name = '成都'
                lookup = json.loads(requests.get(f"https://geoapi.qweather.com/v2/city/lookup?"
                                                 f"location={city_name}&key={KEY}").text)["location"][0]
                self._city_name = lookup["name"]
                self._city_id = lookup["id"]
                self._city_lat = lookup["lat"]
                self._city_lon = lookup["lon"]
                return 1
            except:
                return 0
        return 0
        

    def update_summary(self, timeout=10):
        self._summary.set(json.loads(requests.get(f"https://devapi.qweather.com/v7/minutely/5m?"
                                                  f"location={self._city_lon},{self._city_lat}&key={KEY}").text))

    def update_aqi(self, timeout=10):
        self._aqi.set(json.loads(requests.get(f"https://devapi.qweather.com/v7/air/now?"
                                              f"location={self._city_id}&key={KEY}").text)["now"])

    def update_realtime(self, timeout=10):
        self._realtime.set(json.loads(requests.get(f"https://devapi.qweather.com/v7/weather/now?"
                                                   f"location={self._city_id}&key={KEY}").text)["now"])

    def update_more(self, timeout=10):
        self._weather_more.set(json.loads(
            requests.get(f"https://devapi.qweather.com/v7/weather/3d?"
                         f"location={self._city_id}&key={KEY}").text)["daily"])

    def get_summary(self, ttl=300, timeout=10):
        try:
            if time.time() - self._summary.last_update >= ttl:
                self.update_summary()
            return self._summary.text
        except:
            self.env.Logger.error(traceback.format_exc())
            print(traceback.format_exc())
            return "天气加载失败"

    def get_aqi(self, ttl=600, timeout=10):
        if time.time() - self._aqi.last_update >= ttl:
            self.update_aqi()
        return self._aqi.aqi

    def get_realtime(self, ttl=300, timeout=10):
        if time.time() - self._realtime.last_update >= ttl:
            self.update_realtime()
        return copy.deepcopy(self._realtime)

    def get_more(self, ttl=1200, timeout=10):
        if time.time() - self._weather_more.last_update >= ttl:
            self.update_more()
        return copy.deepcopy(self._weather_more)


